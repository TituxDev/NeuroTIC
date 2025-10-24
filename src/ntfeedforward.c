/**
 * ntfeedforward.c - Feedforward wiring generator for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * This module provides functions to generate `bff_wiring` instructions
 * for feedforward networks. These instructions are used by `buildnet`
 * to allocate and connect neurons, buffers, and outputs.
 *
 * Currently, the module provides:
 *  - feedforward(): Sets up standard feedforward layer-to-layer wiring.
 *
 * Future versions will include additional feedforward variants such as
 * skip connections, dense wiring, and other topologies.
 */

#include "ntfeedforward.h"
#include "ntmemory.h"
#include <stdlib.h>

/**
 * @brief Generate feedforward wiring for a `net_t` network.
 *
 * This function prepares the `bff_wiring` structure for a standard
 * feedforward network:
 *  - Allocates one buffer array per layer connecting outputs to the next layer's inputs.
 *  - Sets `array_type` to 'M' (matrix) for each buffer.
 *  - Configures `size` of each buffer to match the number of neurons in the source layer.
 *  - Sets `src_type`, `src_layer`, and `src_index` so each neuron points to its corresponding output in the previous layer.
 *  - Updates each neuron's `bff_idx` to indicate which buffer it uses.
 *
 * This wiring allows `buildnet` to automatically allocate buffers and connect neurons
 * for a classic feedforward architecture.
 *
 * @param net Pointer to an initialized `net_t` structure.
 *
 * @return Returns the pointer to the same `net_t` with `bff_wiring` configured.
 *
 * @note All memory allocations are tracked using `memtrack()`; no manual `free()` is needed.
 *
 * Example usage:
 *
 *     net_t net;
 *     net.inputs = 3;
 *     net.layers = 3;
 *     unsigned int neurons[3] = { 4, 5, 2 };
 *     newnet(&net, neurons);
 *     feedforward(&net);
 *     buildnet(&net);
 *
 * After execution, `net.bff_wiring` contains wiring for a standard feedforward network.
 */
struct ntnet *newfeedforward( net_t *net ){
    if( !net ) return NULL;
    if( !net->neurons ) return NULL;
    uint16_t L= net->layers - 1;
    memtrack( net->bff_wiring= malloc (L * sizeof( bffwiring_t ) ) );
    for( uint16_t i= 0 ; i < L ; i++ ){
        uint16_t count= net->neurons[i];
        net->bff_wiring[i].arrays= 1;
        memtrack( net->bff_wiring[i].array_type= malloc( sizeof( uint8_t ) ) );
        net->bff_wiring[i].array_type[0]= 'M';
        memtrack( net->bff_wiring[i].size= malloc( sizeof( uint32_t ) ) );
        net->bff_wiring[i].size[0]= count;
        memtrack( net->bff_wiring[i].src_type= malloc( sizeof( uint8_t * ) ) );
        memtrack( net->bff_wiring[i].src_layer= malloc( sizeof( uint16_t * ) ) );
        memtrack( net->bff_wiring[i].src_index= malloc( sizeof( uint16_t * ) ) );
        memtrack( net->bff_wiring[i].src_type[0]= malloc( count * sizeof( uint8_t ) ) );
        memtrack( net->bff_wiring[i].src_layer[0]= malloc( count * sizeof( uint16_t ) ) );
        memtrack( net->bff_wiring[i].src_index[0]= malloc( count * sizeof( uint16_t ) ) );
        for( uint16_t j= 0 ; j < count ; j++ ){
            net->bff_wiring[i].src_type[0][j]= 'N';
            net->bff_wiring[i].src_layer[0][j]= i;
            net->bff_wiring[i].src_index[0][j]= j;
            net->nn[i][j].bff_idx= 0;
        }
    }
    return net;
}

struct ntnet *newdense( net_t *net ){
    if( !net ) return NULL;
    if( !net->neurons ) return NULL;
    uint16_t L= net->layers - 1;
    memtrack( net->bff_wiring= malloc (L * sizeof( bffwiring_t ) ) );
    uint32_t count= 0;
    for( uint16_t i= 0 ; i < L ; i++ ){
        count+= net->neurons[i];
        net->bff_wiring[i].arrays= 1;
        memtrack( net->bff_wiring[i].array_type= malloc( sizeof( uint8_t ) ) );
        net->bff_wiring[i].array_type[0]= 'M';
        memtrack( net->bff_wiring[i].size= malloc( sizeof( uint32_t ) ) );
        net->bff_wiring[i].size[0]= count;
        memtrack( net->bff_wiring[i].src_type= malloc( sizeof( uint8_t * ) ) );
        memtrack( net->bff_wiring[i].src_layer= malloc( sizeof( uint16_t * ) ) );
        memtrack( net->bff_wiring[i].src_index= malloc( sizeof( uint16_t * ) ) );
        memtrack( net->bff_wiring[i].src_type[0]= malloc( count * sizeof( uint8_t ) ) );
        memtrack( net->bff_wiring[i].src_layer[0]= malloc( count * sizeof( uint16_t ) ) );
        memtrack( net->bff_wiring[i].src_index[0]= malloc( count * sizeof( uint16_t ) ) );
        uint16_t layer= 0;
        uint16_t index= 0;
        for( uint32_t j= 0 ; j < count ; j++ ){
            net->bff_wiring[i].src_type[0][j]= 'N';
            net->bff_wiring[i].src_layer[0][j]= layer;
            net->bff_wiring[i].src_index[0][j]= index;
            layer+= !!(index= index < net->neurons[layer] ? index + 1 : 0);
        }
        for( uint16_t j= 0 ; j < net->neurons[i] ; j++ ) net->nn[i][j].bff_idx= 0;
    }
    return net;
}