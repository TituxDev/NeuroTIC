/**
 * @file ntfeedforward.c
 * @brief Implementation of predefined network topologies.
 *
 * Provides functions to generate common wiring configurations for NeuroTIC
 * networks, initializing `bff_wiring` and neuron `bff_idx` values.
 *
 * This module is intended for convenience; users may also define their own
 * custom `bff_wiring` structures in the main code.
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#include "ntfeedforward.h"
#include "ntmemory.h"
#include <stdlib.h>

/**
 * @brief Initializes a simple feedforward topology.
 *
 * - Allocates memory for `bff_wiring`.
 * - Sets one buffer per neuron connecting it to the corresponding neuron
 *   in the previous layer.
 * - Initializes neuron `bff_idx` to 0.
 *
 * @param net Pointer to the network to configure.
 * @return Pointer to the configured network, or NULL on invalid input.
 */
struct net_s *newfeedforward( net_s *net ){
    if( !net ) return NULL;
    if( !net->neurons ) return NULL;
    layer_t L= net->layers - 1;
    memtrack( net->wiring= malloc (L * sizeof( wiring_s ) ) );
    for( layer_t i= 0 ; i < L ; i++ ){
        uint16_t count= net->neurons[i];
        net->wiring[i].arrays= 1;
        memtrack( net->wiring[i].array_type= malloc( sizeof( type_t ) ) );
        net->wiring[i].array_type[0]= 'M';
        memtrack( net->wiring[i].size= malloc( sizeof( input_t ) ) );
        net->wiring[i].size[0]= count;
        memtrack( net->wiring[i].src_type= malloc( sizeof( type_t * ) ) );
        memtrack( net->wiring[i].src_layer= malloc( sizeof( layer_t * ) ) );
        memtrack( net->wiring[i].src_index= malloc( sizeof( uint16_t * ) ) );
        memtrack( net->wiring[i].src_type[0]= malloc( count * sizeof( type_t ) ) );
        memtrack( net->wiring[i].src_layer[0]= malloc( count * sizeof( layer_t ) ) );
        memtrack( net->wiring[i].src_index[0]= malloc( count * sizeof( uint16_t ) ) );
        for( uint16_t j= 0 ; j < count ; j++ ){
            net->wiring[i].src_type[0][j]= 'N';
            net->wiring[i].src_layer[0][j]= i;
            net->wiring[i].src_index[0][j]= j;
            net->nn[i][j].bff_idx= 0;
        }
    }
    return net;
}

/**
 * @brief Initializes a dense feedforward topology.
 *
 * - Allocates memory for `bff_wiring`.
 * - Connects each neuron in a layer to all neurons in previous layers.
 * - Sets neuron `bff_idx` to 0.
 *
 * @param net Pointer to the network to configure.
 * @return Pointer to the configured network, or NULL on invalid input.
 */
struct net_s *newdense( net_s *net ){
    if( !net ) return NULL;
    if( !net->neurons ) return NULL;
    layer_t L= net->layers - 1;
    memtrack( net->wiring= malloc (L * sizeof( wiring_s ) ) );
    input_t count= 0;
    for( uint16_t i= 0 ; i < L ; i++ ){
        count+= net->neurons[i];
        net->wiring[i].arrays= 1;
        memtrack( net->wiring[i].array_type= malloc( sizeof( uint8_t ) ) );
        net->wiring[i].array_type[0]= 'M';
        memtrack( net->wiring[i].size= malloc( sizeof( uint32_t ) ) );
        net->wiring[i].size[0]= count;
        memtrack( net->wiring[i].src_type= malloc( sizeof( type_t * ) ) );
        memtrack( net->wiring[i].src_layer= malloc( sizeof( layer_t * ) ) );
        memtrack( net->wiring[i].src_index= malloc( sizeof( uint16_t * ) ) );
        memtrack( net->wiring[i].src_type[0]= malloc( count * sizeof( type_t ) ) );
        memtrack( net->wiring[i].src_layer[0]= malloc( count * sizeof( layer_t ) ) );
        memtrack( net->wiring[i].src_index[0]= malloc( count * sizeof( uint16_t ) ) );
        layer_t layer= 0;
        uint16_t index= 0;
        for( input_t j= 0 ; j < count ; j++ ){
            net->wiring[i].src_type[0][j]= 'N';
            net->wiring[i].src_layer[0][j]= layer;
            net->wiring[i].src_index[0][j]= index;
            layer+= !!(index= index < net->neurons[layer] ? index + 1 : 0);
        }
        for( uint16_t j= 0 ; j < net->neurons[i] ; j++ ) net->nn[i][j].bff_idx= 0;
    }
    return net;
}