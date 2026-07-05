/**
 * @file ntfeedforward.c
 * @brief Implementation of predefined feedforward topologies for NeuroTIC networks.
 *
 * @details
 * Provides ready-made functions that allocate and populate each layer's
 * wiring descriptor and configure every neuron's buffer selector according
 * to a specific connectivity pattern.
 *
 * @author Oscar Sotomayor
 * @date 2026
 */

#include "ntfeedforward.h"
#include "ntmemory.h"
#include <stdlib.h>

/**
 * @details
 * For each layer boundary, builds a single 'M'-type wiring array whose
 * slots enumerate every neuron of the current layer as an 'N' source, then
 * points every neuron of the next layer at that shared buffer (bff_idx = 0)
 * -- fully connecting each layer to only the one immediately preceding it.
 *
 * @retval NULL
 *  - `net` is NULL.
 *  - `net->neurons` has not been allocated yet.
 */
struct net_s *newfeedforward( net_s *net ){
    if( !net ) return NULL;
    if( !net->neurons ) return NULL;
    layer_t L= net->layers - 1;
    for( layer_t i= 0 ; i < L ; i++ ){
        uint16_t count= net->neurons[i];
        net->wiring[i].arrays= 1;
        net->wiring[i].array_type= createregister( net , malloc( sizeof( type_t ) ) );
        net->wiring[i].array_type[0]= 'M';
        net->wiring[i].size= createregister( net , malloc( sizeof( input_t ) ) );
        net->wiring[i].size[0]= count;
        net->wiring[i].src_type= createregister( net , malloc( sizeof( type_t * ) ) );
        net->wiring[i].src_layer= createregister( net , malloc( sizeof( layer_t * ) ) );
        net->wiring[i].src_index= createregister( net , malloc( sizeof( uint16_t * ) ) );
        net->wiring[i].src_type[0]= createregister( net , malloc( count * sizeof( type_t ) ) );
        net->wiring[i].src_layer[0]= createregister( net , malloc( count * sizeof( layer_t ) ) );
        net->wiring[i].src_index[0]= createregister( net , malloc( count * sizeof( uint16_t ) ) );
        for( uint16_t j= 0 ; j < count ; j++ ){
            net->wiring[i].src_type[0][j]= 'N';
            net->wiring[i].src_layer[0][j]= i;
            net->wiring[i].src_index[0][j]= j;
        }
        for( uint16_t j= 0 ; j < net->neurons[i + 1] ; j++ ) net->nn[i + 1][j].bff_idx= 0;
    }
    return net;
}

/**
 * @details
 * For each layer boundary, builds a single 'M'-type wiring array whose
 * slots enumerate every neuron from layer 0 up to and including the
 * current layer as an 'N' source -- the input set grows with each
 * boundary, so every layer ends up connected to all preceding layers, not
 * just the one immediately before it. Every neuron of the next layer is
 * pointed at that shared buffer (bff_idx = 0).
 *
 * @retval NULL
 *  - `net` is NULL.
 *  - `net->neurons` has not been allocated yet.
 */
struct net_s *newdense( net_s *net ){
    if( !net ) return NULL;
    if( !net->neurons ) return NULL;
    layer_t L= net->layers - 1;
    input_t count= 0;
    for( uint16_t i= 0 ; i < L ; i++ ){
        count+= net->neurons[i];
        net->wiring[i].arrays= 1;
        net->wiring[i].array_type= createregister( net , malloc( sizeof( uint8_t ) ) );
        net->wiring[i].array_type[0]= 'M';
        net->wiring[i].size= createregister( net , malloc( sizeof( uint32_t ) ) );
        net->wiring[i].size[0]= count;
        net->wiring[i].src_type= createregister( net , malloc( sizeof( type_t * ) ) );
        net->wiring[i].src_layer= createregister( net , malloc( sizeof( layer_t * ) ) );
        net->wiring[i].src_index= createregister( net , malloc( sizeof( uint16_t * ) ) );
        net->wiring[i].src_type[0]= createregister( net , malloc( count * sizeof( type_t ) ) );
        net->wiring[i].src_layer[0]= createregister( net , malloc( count * sizeof( layer_t ) ) );
        net->wiring[i].src_index[0]= createregister( net , malloc( count * sizeof( uint16_t ) ) );
        layer_t layer= 0;
        uint16_t index= 0;
        for( input_t j= 0 ; j < count ; j++ ){
            net->wiring[i].src_type[0][j]= 'N';
            net->wiring[i].src_layer[0][j]= layer;
            net->wiring[i].src_index[0][j]= index;
            layer+= !(index= index + 1 < net->neurons[layer] ? index + 1 : 0);
        }
        for( uint16_t j= 0 ; j < net->neurons[i + 1] ; j++ ) net->nn[i + 1][j].bff_idx= 0;
    }
    return net;
}