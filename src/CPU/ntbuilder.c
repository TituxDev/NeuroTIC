/**
 * @file ntbuilder.c
 * @brief Implementation of network construction functions.
 *
 * Provides memory allocation and pointer setup for neurons, input/output references, and inter-layer buffers.  
 * Uses memtrack() to manage memory safely.
 * 
 * @author Oscar Sotomayor
 * @date 2026
 */

#include "ntbuilder.h"
#include "ntmemory.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @retval NULL
 *  - `net` or `neurons_per_layer` is NULL.
 *  - net_s::layers is less than 1.
 *  - `layers_size` does not match net_s::layers.
 *  - any per-layer neuron count is less than 1.
 *
 * @details
 * Copies `neurons_per_layer` into net_s::neurons and allocates net_s::nn with one
 * neuron_s array per layer.
 *
 * When net_s::layers is greater than 1, allocates one net_s::wiring descriptor
 * per layer transition.
 *
 * Initializes net_s::in, net_s::bff and net_s::out to `NULL`. These structures
 * are allocated and resolved later by buildnet().
 */
struct net_s *newnet( net_s *net , uint16_t *neurons_per_layer , layer_t layers_size ){
    if( !net || !neurons_per_layer || net->layers < 1 || layers_size != net->layers ) return NULL;
    for( layer_t i = 0 ; i < net->layers ; i++ ) if( neurons_per_layer[i] < 1 ) return NULL;
    net->neurons= createregister( (void *)net , calloc( net->layers , sizeof( uint16_t ) ) );
    memcpy( net->neurons , neurons_per_layer, net->layers * sizeof( uint16_t ) );
    net->nn= createregister( (void *)net , calloc( net->layers , sizeof( neuron_s * ) ) );
    for( uint16_t i = 0 ; i < net->layers ; i++ ) net->nn[i]= createregister( (void *)net , calloc( net->neurons[i] , sizeof( neuron_s ) ) );
    net->wiring= net->layers > 1 ? createregister( (void *)net , calloc( net->layers - 1 , sizeof( wiring_s ) ) ) : NULL;
    net->in= NULL;
    net->bff= NULL;
    net->out= NULL;
    return net;
}

/**
 * @retval 1 Argument is NULL.
 * @retval 0 SUCESS
 *
 * @details
 * Defines neuron_s::inputs for every neuron according to its position in the
 * network.
 *
 * Neurons on the first layer use net_s::inputs as their input count.
 *
 * Neurons from the second layer onward use the resolved net_s::wiring descriptor
 * selected by neuron_s::bff_idx. The corresponding wiring_s::size value defines
 * the number of inputs for each neuron.
 */
uint8_t defineneurons( net_s *net ){
    if( !net ) return 1;
    for( uint16_t i= 0 ; i < net->neurons[0] ; i++ ) net->nn[0][i].inputs= net->inputs;
    for( layer_t i= 1, f= net->layers; i < f ; i++ ) for(uint16_t j= 0 ; j < net->neurons[i] ; j++) net->nn[i][j].inputs= net->wiring[i-1].size[net->nn[i][j].bff_idx];
    return 0;
}

/**
 * @retval 1 Argument is NULL.
 * @retval 0 SUCESS
 *
 * @details
 * Allocates the weight array for every neuron in the network.
 *
 * The number of weights allocated for each neuron is determined by
 * neuron_s::inputs, which must have been defined previously by defineneurons().
 */
uint8_t buildneurons( net_s *net ){
    if( !net ) return 1;
    for( layer_t i= 0 ; i < net->layers ; i ++ ) for( uint16_t j= 0 ; j < net->neurons[i] ; j++ ) net->nn[i][j].w= createregister( (void *)net , calloc( net->nn[i][j].inputs , sizeof( weight_t ) ) );
    return 0;
}

/**
 * @retval NULL
 *  - `net` is NULL.
 *  - `net->neurons` has not been allocated yet.
 *
 * @details
 * If net_s::layers > 1, resolves each net_s::wiring descriptor into a net_s::bff array
 * for its corresponding layer transition (`net->bff[0..layers-2]`), in two passes:
 * - First pass: for arrays with wiring_s::array_type `'M'`, each element is resolved
 *   individually according to its own wiring_s::src_type.
 * - Second pass: for all other arrays, the whole array is resolved at once according to
 *   its wiring_s::array_type, and wiring_s::size is updated to match the resolved size.
 * Layer 0 is not processed by either pass.
 *
 * Then, calls defineneurons() to define neuron_s::inputs for every neuron
 * according to its layer and selected wiring_s::size entry.
 *
 * Calls buildneurons() to allocate neuron_s::w according to the resolved
 * neuron_s::inputs values.
 *
 * Finally, sets neuron_s::in for every neuron from layer 1 onward using the
 * net_s::bff entry selected by neuron_s::bff_idx, while neurons on layer 0
 * receive net_s::in.
 *
 * @note
 * wiring_s::array_type (second pass):
 *      - 'I': Alias to net_s::in.
 *      - 'O': Alias to net_s::out.
 *      - 'N': Alias to another already-resolved net_s::bff array, referenced
 *        by this array's own wiring_s::src_layer[0] and wiring_s::src_index[0].
 *      - any other value: left as `NULL`, with a placeholder size of `1`.
 *
 * @note
 * wiring_s::src_type, for elements inside a `'M'` array (first pass):
 *      - 'N': `net->nn[src_layer][src_index].out`.
 *      - 'O': A single element of net_s::out.
 *      - any other value: left as `NULL`.
 */
struct net_s *buildnet( net_s *net ){ 
    if( !net ) return net;
    if( !net->neurons ) NULL;
    net->in= createregister( (void *)net , calloc( net->inputs , sizeof( data_t * ) ) );
    net->out= createregister( (void *)net , calloc( net->neurons[net->layers - 1] , sizeof( data_t * ) ) );
    for( uint16_t i = 0 ; i < net->neurons[net->layers - 1] ; i++ ) net->out[i]= &net->nn[net->layers - 1][i].out;
    if( net->wiring ){
        net->bff= createregister( (void *)net , calloc( net->layers - 1 , sizeof( data_t *** ) ) );
        for( layer_t i= 0 ; i < net->layers - 1 ; i++ ){
            net->bff[i]= createregister( (void *)net , calloc( net->wiring[i].arrays , sizeof( data_t ** ) ) );
            for( input_t j= 0 ; j < net->wiring[i].arrays ; j++ ){
                if( net->wiring[i].array_type[j] == 'M' ){
                    net->bff[i][j]= createregister( (void *)net , calloc( net->wiring[i].size[j] , sizeof( data_t * ) ) );
                    for( uint32_t k= 0 ; k < net->wiring[i].size[j] ; k++ ){
                        switch( net->wiring[i].src_type[j][k] ){
                            case 'N':
                                net->bff[i][j][k]= &net->nn[net->wiring[i].src_layer[j][k]][net->wiring[i].src_index[j][k]].out;
                                break;
                            case 'O':
                                net->bff[i][j][k]= net->out[net->wiring[i].src_index[j][k]];
                                break;
                            default:
                                net->bff[i][j][k]= NULL;
                                break;
                        }
                    }
                }
            }
        }
        for( layer_t i= 0 ; i < net->layers - 1 ; i++) for( uint16_t j = 0 ; j < net->wiring[i].arrays ; j++) switch( net->wiring[i].array_type[j] ){
            case 'M':
                break;
            case 'N':
                net->bff[i][j]= net->bff[net->wiring[i].src_layer[j][0]][net->wiring[i].src_index[j][0]];
                net->wiring[i].size[j]= net->wiring[net->wiring[i].src_layer[j][0]].size[net->wiring[i].src_index[j][0]];
                break;
            case 'I':
                net->bff[i][j]= net->in;
                net->wiring[i].size[j]= net->inputs;
                break;
            case 'O':
                net->bff[i][j]= net->out;
                net->wiring[i].size[j]= net->neurons[net->layers - 1];
                break;
            default:
                net->bff[i][j]= NULL;
                net->wiring[i].size[j]= 1;
                break;
        }
    }
    defineneurons( net );
    buildneurons( net );
    for( layer_t i= 0 ; i < net->layers ; i++ ) for( uint16_t j= 0 ; j < net->neurons[i] ; j++) net->nn[i][j].in= i ? net->bff[i - 1][net->nn[i][j].bff_idx] : net->in;
    return net;
}