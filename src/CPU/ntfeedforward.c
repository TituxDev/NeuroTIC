/**
 * @file ntfeedforward.c
 * @brief Implementation of predefined feedforward topologies for NeuroTIC networks.
 * 
 * Provides convenient functions to initialize common neural network wiring configurations.  
 * These functions allocate and configure the network's `bff_wiring` structure according to a specific topology.
 * 
 * @author Oscar Sotomayor
 * @date 2026
 */

#include "ntfeedforward.h"
#include "ntmemory.h"
#include <stdlib.h>

/**
 * @details
 * Sets up one buffer per neuron connecting each neuron to the corresponding neuron in the previous layer.  
 * Validates input and uses `memtrack` for all allocations.
 */
struct net_s *newfeedforward( net_s *net ){
    if( !net ) return NULL;
    if( !net->neurons ) return NULL;
    layer_t L= net->layers - 1;
    net->wiring= memtrack( malloc (L * sizeof( wiring_s ) ) );
    for( layer_t i= 0 ; i < L ; i++ ){
        uint16_t count= net->neurons[i];
        net->wiring[i].arrays= 1;
        net->wiring[i].array_type= memtrack( malloc( sizeof( type_t ) ) );
        net->wiring[i].array_type[0]= 'M';
        net->wiring[i].size= memtrack( malloc( sizeof( input_t ) ) );
        net->wiring[i].size[0]= count;
        net->wiring[i].src_type= memtrack( malloc( sizeof( type_t * ) ) );
        net->wiring[i].src_layer= memtrack( malloc( sizeof( layer_t * ) ) );
        net->wiring[i].src_index= memtrack( malloc( sizeof( uint16_t * ) ) );
        net->wiring[i].src_type[0]= memtrack( malloc( count * sizeof( type_t ) ) );
        net->wiring[i].src_layer[0]= memtrack( malloc( count * sizeof( layer_t ) ) );
        net->wiring[i].src_index[0]= memtrack( malloc( count * sizeof( uint16_t ) ) );
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
 * @details
 * Connects each neuron in a layer to all neurons in previous layers, creating a fully connected inter-layer mapping.  
 * Validates input and uses `memtrack` for all allocations.
 */
struct net_s *newdense( net_s *net ){
    if( !net ) return NULL;
    if( !net->neurons ) return NULL;
    layer_t L= net->layers - 1;
    net->wiring= memtrack( malloc (L * sizeof( wiring_s ) ) );
    input_t count= 0;
    for( uint16_t i= 0 ; i < L ; i++ ){
        count+= net->neurons[i];
        net->wiring[i].arrays= 1;
        net->wiring[i].array_type= memtrack( malloc( sizeof( uint8_t ) ) );
        net->wiring[i].array_type[0]= 'M';
        net->wiring[i].size= memtrack( malloc( sizeof( uint32_t ) ) );
        net->wiring[i].size[0]= count;
        net->wiring[i].src_type= memtrack( malloc( sizeof( type_t * ) ) );
        net->wiring[i].src_layer= memtrack( malloc( sizeof( layer_t * ) ) );
        net->wiring[i].src_index= memtrack( malloc( sizeof( uint16_t * ) ) );
        net->wiring[i].src_type[0]= memtrack( malloc( count * sizeof( type_t ) ) );
        net->wiring[i].src_layer[0]= memtrack( malloc( count * sizeof( layer_t ) ) );
        net->wiring[i].src_index[0]= memtrack( malloc( count * sizeof( uint16_t ) ) );
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