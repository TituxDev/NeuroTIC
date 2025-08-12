/**
 * ntbuilder.c - Neural network construction implementation for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * This file contains implementations of functions to initialize and build
 * neural networks in the `net_t` structure. It handles dynamic memory allocation
 * for neurons, layers, connections, and internal buffers using the memory tracking
 * module to ensure automatic resource management.
 *
 * The main functions allow:
 *  - Initializing the network structure with the number of layers and neurons per layer.
 *  - Fully building the network by allocating neurons, connections, and setting up pointers
 *    for data flow.
 */

#include "ntbuilder.h"

#include "ntmemory.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Initializes the `net_t` structure with neurons per layer count.
 *
 * Allocates memory for the array holding the number of neurons in each layer,
 * and copies the data from the user-provided array.
 *
 * Internal pointers (`in`, `nn`, `bff`, `out`) are set to NULL,
 * leaving full construction to the `buildnet` function.
 *
 * @param net Pointer to a pre-declared `net_t` structure.
 * @param neurons_per_layer Pointer to an array with neuron counts per layer.
 * 
 * @return Returns the pointer to the initialized `net_t` structure.
 */
struct ntnet *newnet( net_t *net , unsigned int *neurons_per_layer ){
    net->in = NULL;
    net->nn = NULL;
    net->bff = NULL;
    net->out = NULL;
    memtrack( net->neurons= malloc( net->layers * sizeof( int ) ) );
    memcpy( net->neurons , neurons_per_layer , net->layers * sizeof( int ) );
    return net;
}

/**
 * @brief Builds the neural network in memory by allocating and connecting neurons, layers, and buffers.
 *
 * This function performs the following tasks:
 *  - Allocates memory for the neuron matrix (`nn`), with layers and neurons per layer.
 *  - Allocates memory for the input pointers array (`in`).
 *  - Initializes neurons in the first layer, assigning inputs and weights.
 *  - Allocates intermediate buffers (`bff`) to connect consecutive layers.
 *  - Sets inputs, weights, and connections for hidden and output layer neurons.
 *  - Allocates and assigns the final output pointers array (`out`).
 *
 * All allocated blocks are registered in the memory tracking module
 * for automatic freeing.
 *
 * @param net Pointer to a previously initialized `net_t` structure (usually via `newnet`).
 *
 * @return Returns the pointer to the fully built `net_t` structure.
 */
struct ntnet *buildnet( net_t *net ){
    memtrack( net->nn= malloc( net->layers * sizeof( neuron_t * ) ) );
    for( unsigned int i= 0 ; i < net->layers ; i++ ) memtrack( net->nn[i]= malloc( net->neurons[i] * sizeof( neuron_t ) ) );
    memtrack(net->in = malloc(net->inputs * sizeof(float *)));
    for( unsigned int i= 0 ; i < net->neurons[0] ; i++ ){
        net->nn[0][i].inputs= net->inputs;
        net->nn[0][i].in= net->in;
        memtrack( net->nn[0][i].w= malloc( net->inputs * sizeof( float ) ) );
    }
    memtrack( net->bff= malloc( ( net->layers - 1 ) * sizeof( float ** ) ) );
    for(unsigned int i = 0, j, f=net->layers - 1 ; i < f ; i++ ){
        memtrack( net->bff[i]= malloc( net->neurons[i] * sizeof( float * ) ) );
        for ( j= 0 ; j < net->neurons[i] ; j++ ) net->bff[i][j]= &net->nn[i][j].out;
    }
    for( unsigned int i= 1, j, bff_ly ; i < net->layers ; i++ ) for( j= 0, bff_ly= i - 1 ; j < net->neurons[i] ; j++ ){
        net->nn[i][j].inputs= net->neurons[bff_ly];
        net->nn[i][j].in= net->bff[bff_ly];
        memtrack( net->nn[i][j].w= malloc( net->neurons[bff_ly] * sizeof( float ) ) );
    }
    unsigned int last_ly= net->layers - 1;
    memtrack( net->out= malloc( net->neurons[last_ly] * sizeof( float * ) ) );
    for( unsigned int i= 0 ; i < net->neurons[last_ly] ; i++ ) net->out[i]= &net->nn[last_ly][i].out;
    return net;
}
