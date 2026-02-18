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
 */
data_t activate( neuron_s *neuron ){
    return neuron->out= ntact_activation[neuron->fn][0]( weighing( neuron ) );
}


/**
 * @details
 * Iterates layer-by-layer to maintain deterministic ordering.  
 * No buffering is required since outputs of previous layers are already wired via pointer connections.
 */
data_t *feedforward( net_s *net ){
    for( layer_t i= 0, j ; i < net->layers ; i++ ) for( j= 0 ; j < net->neurons[i] ; j++ ) activate( &net->nn[i][j] );
    return *net->out;
}
