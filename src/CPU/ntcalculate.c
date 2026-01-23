/**
 * @file ntcalculate.c
 * @brief Implementation of feedforward network computations.
 *
 * Provides weighted sum, activation, and full forward propagation for
 * NeuroTIC networks. Assumes sequential layer-by-layer calculation.
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#include "ntcalculate.h"
#include "ntactivation.h"

/**
 * @brief Computes weighted sum plus bias for a neuron.
 *
 * @param neuron Pointer to the neuron.
 * @return Weighted sum of inputs plus bias.
 */
float weighing( neuron_s *neuron ){
    float wgh= neuron->b;
    for( uint32_t i = 0 ; i < neuron->inputs ; i++ ) wgh+= *neuron->in[i] * neuron->w[i];
    return wgh;
}

/**
 * @brief Applies the activation function to a neuron.
 *
 * Stores the activated value in neuron->out.
 *
 * @param neuron Pointer to the neuron.
 * @return Activated output value.
 */
float activate( neuron_s *neuron ){
    return neuron->out= ntact_activation[neuron->fn][0]( weighing( neuron ) );
}


/**
 * @brief Performs full feedforward computation for the network.
 *
 * Iterates over all layers and neurons sequentially, activating each neuron.
 *
 * @param net Pointer to the network to compute.
 * @return Pointer to the final layer's output array.
 */
float *feedforward( net_s *net ){
    for( uint16_t i = 0, j ; i < net->layers ; i++ ) for( j = 0 ; j < net->neurons[i] ; j++ ) activate( &net->nn[i][j] );
    return *net->out;
}
