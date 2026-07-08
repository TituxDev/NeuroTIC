/**
 * @file ntcalculate.c
 * @brief Implementation of feedforward network computations.
 *
 * @details
 * Provides weighted sum, activation, and full forward propagation.  
 * Assumes sequential layer-by-layer calculation.
 * 
 * @author Oscar Sotomayor
 * @date 2026
 */

#include "ntcalculate.h"
#include "ntactivation.h"
#include <stdlib.h>

/**
 * @details
 * Calculates the weighted sum of a neuron by iterating over its inputs and weights.  
 * weighted_sum = Σ(input_i * weight_i) + bias  
 * The bias term is accumulated first to reduce one addition inside the loop.  
 * Inputs are assumed to be valid pointers.
 */
data_t weighing( neuron_s *neuron ){
    data_t wgh= neuron->b;
    for( input_t i= 0 ; i < neuron->inputs ; i++ ) wgh+= *neuron->in[i] * neuron->w[i];
    return wgh;
}

/**
 * @details
 * Activation functions are resolved from the global `ntact_activation` lookup table using the neuron's configured activation identifier.  
 * Result is stored in the neuron's `out` field.
 *
 * @todo `neuron_s::fn` is not validated against `NTACT_TOTAL_FUNCTIONS`
 * before indexing `ntact_activation` -- behavior for an out-of-range value
 * is undefined until a validation strategy (and how tightly coupled it
 * should be with other modules) is decided.
 */
data_t activate( neuron_s *neuron ){
    return neuron->out= ntact_activation[neuron->fn][0]( weighing( neuron ) );
}


/** 
 * @retval NULL `net` is NULL.
 *
 * @details
 * Iterates layer-by-layer to maintain deterministic ordering. Before
 * evaluating a neuron, re-resolves any `'I'`-typed element nested inside
 * its selected `'M'` buffer against the network's current `net_s::in` --
 * every other wiring reference was already resolved once, permanently, by
 * `buildnet()`. All other buffer entries are read as-is via their existing
 * pointer connections.
 */
data_t **feedforward( net_s *net ){
    if( !net ) return NULL;
    for( layer_t i= 0 ; i < net->layers ; i++ ){
        for( uint16_t j= 0 ; j < net->neurons[i] ; j++ ){
            if( i && net->wiring[i - 1].array_type[net->nn[i][j].bff_idx] == 'M' ) for( uint32_t k= 0 ; k < net->nn[i][j].inputs ; k++ ) switch( net->wiring[i-1].src_type[net->nn[i][j].bff_idx][k] ){
                case 'I':
                    net->bff[i - 1][net->nn[i][j].bff_idx][k]= net->in[net->wiring[i - 1].src_index[net->nn[i][j].bff_idx][k]];
                    break;
            }
            activate( &net->nn[i][j] );
        }
    }
    return net->out;
}