/**
 * ntbuilder.c - Neural network construction implementation for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * This module implements the initialization and construction of neural networks
 * within the `net_t` structure. It dynamically allocates memory for neurons,
 * layers, connections, and intermediate buffers, registering all allocations
 * with the memory tracking system for automatic cleanup at program termination.
 *
 * Provided functions:
 *  - `newnet()`  : Initializes a network structure with a given neuron layout.
 *  - `buildnet()`: Fully builds the network by creating connections, buffers,
 *                  and setting up neuron input pointers.
 */

#include "ntbuilder.h"
#include "ntmemory.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Initializes a `net_t` structure with the specified neuron distribution.
 *
 * This function:
 *  - Allocates and stores the number of neurons for each layer.
 *  - Allocates the neuron matrix (`nn`) for each layer.
 *  - Leaves all connection and buffer pointers (`in`, `bff`, `out`) uninitialized.
 *
 * Memory allocated is automatically registered for later release by `memfree()`.
 *
 * @param net Pointer to an already allocated `net_t` structure with `layers` set.
 * @param neurons_per_layer Pointer to an array containing neuron counts per layer.
 *
 * @return Pointer to the initialized `net_t` structure.
 */
struct ntnet *newnet( net_t *net , uint16_t *neurons_per_layer ){
    net->in= NULL;
    net->nn= NULL;
    net->bff= NULL;
    net->out= NULL;
    memtrack( net->neurons= malloc( net->layers * sizeof( uint16_t ) ) );
    memcpy( net->neurons , neurons_per_layer, net->layers * sizeof( uint16_t ) );
    memtrack( net->nn= malloc( net->layers * sizeof( neuron_t * ) ) );
    for( uint16_t i = 0 ; i < net->layers ; i++ ) memtrack( net->nn[i]= malloc( net->neurons[i] * sizeof( neuron_t ) ) );
    return net;
}

/**
 * @brief Fully constructs a neural network in memory.
 *
 * Performs allocation and wiring of all network components:
 *  - Allocates input pointer array (`in`).
 *  - Allocates buffer pointer arrays (`bff`) between layers based on `bff_wiring`.
 *  - Allocates output pointer array (`out`).
 *  - Assigns input pointers and weights for neurons in the first layer.
 *  - Connects buffer arrays and assigns neuron input sources according to `bff_wiring`.
 *
 * This function relies on a valid `bff_wiring` configuration in the `net_t`
 * structure to determine how intermediate buffers and connections are set up.
 *
 * All allocations are tracked via `memtrack()` for automatic cleanup.
 *
 * @param net Pointer to a previously initialized `net_t` structure.
 *
 * @return Pointer to the fully built `net_t` structure.
 */
struct ntnet *buildnet( net_t *net ){
    memtrack( net->in = malloc( net->inputs * sizeof( float * ) ) );
    memtrack( net->bff= malloc( ( net->layers - 1 ) * sizeof( float *** ) ) );
    for( uint16_t i= 0, f= net->layers - 1 ; i < f ; i++ ){
        memtrack( net->bff[i]= malloc( net->bff_wiring[i].arrays * sizeof( float ** ) ) );
        for( uint16_t j= 0 ; j < net->bff_wiring[i].arrays ; j++)
            if( net->bff_wiring[i].array_type[j] == 'M' ) memtrack( net->bff[i][j]= malloc( net->bff_wiring[i].size[j] * sizeof( float * ) ) );
            else net->bff[i][j]= NULL;
    }
    memtrack( net->out= malloc( net->neurons[net->layers - 1] * sizeof( float * ) ) );
    for( uint16_t i= 0 ; i < net->neurons[0] ; i++ ){
        net->nn[0][i].inputs= net->inputs;
        net->nn[0][i].in= net->in;
        memtrack( net->nn[0][i].w= malloc( net->inputs * sizeof( float ) ) );
    }
    for( uint16_t i= 0, f= net->layers - 1 ; i < f ; i++ ) for( uint16_t j= 0 ; j < net->bff_wiring[i].arrays ; j++ )
        if( net->bff[i][j] )
            for( uint32_t k= 0 ; k < net->bff_wiring[i].size[j] ; k++ )
                net->bff[i][j][k]=
                    net->bff_wiring[i].src_type[j][k] == 'N' ?
                        &net->nn[net->bff_wiring[i].src_layer[j][k]][net->bff_wiring[i].src_index[j][k]].out :
                    net->bff_wiring[i].src_type[j][k] == 'I' ?
                        net->in[net->bff_wiring[i].src_index[j][k]] :
                    net->bff_wiring[i].src_type[j][k] == 'O' ?
                        net->out[net->bff_wiring[i].src_index[j][k]]
                    : NULL;
        else 
            net->bff[i][j]=
                net->bff_wiring[i].array_type[j] == 'N' ?
                    net->bff[net->bff_wiring[i].src_layer[j][0]][net->bff_wiring[i].src_index[j][0]] :
                net->bff_wiring[i].array_type[j] == 'I' ?
                    net->in :
                net->bff_wiring[i].array_type[j] == 'O' ?
                    net->out
                : NULL;
    for( uint16_t i= 1 ; i < net->layers ; i++ ) for( uint16_t j= 0 ; j < net->neurons[i] ; j++ ) net->nn[i][j].in= net->bff[i][net->nn[i][j].bff_idx];
    return net;
}
