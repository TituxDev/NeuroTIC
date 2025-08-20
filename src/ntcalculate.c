/**
 * ntcalculate.c - Feedforward calculation functions for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * Implements core functions to compute neuron weighted sums, apply activation
 * functions, and perform feedforward propagation through the network.
 *
 * Currently focused on feedforward computation, but designed for easy extension
 * to other computational models.
 */

#include "ntcalculate.h"
#include "ntactivation.h"

/**
 * @brief Calculate the weighted sum of inputs plus bias for a neuron.
 *
 * Multiplies each input by its corresponding weight and sums them, adding
 * the neuron's bias.
 *
 * @param neuron Pointer to the neuron structure.
 * @return The weighted sum (float).
 */
float weighing( neuron_t *neuron ){
    float wgh= neuron->b;
    for( uint32_t i = 0 ; i < neuron->inputs ; i++ ) wgh+= *neuron->in[i] * neuron->w[i];
    return wgh;
}

/**
 * @brief Apply the activation function to the weighted sum and store output.
 *
 * Calls the activation function specified by the neuron’s `fn` index on the
 * weighted sum and updates the neuron’s output value.
 *
 * @param neuron Pointer to the neuron structure.
 * @return The activated output value (float).
 */
float activate( neuron_t *neuron ){
    return neuron->out= ntact_activation[neuron->fn][0]( weighing( neuron ) );
}

/**
 * @brief Perform feedforward computation over all neurons in the network.
 *
 * Iterates over each layer and neuron, computes and activates outputs in order.
 *
 * @param net Pointer to the neural network structure.
 * @return Pointer to the array of output neuron values of the last layer.
 */
float *feedforward( net_t *net ){
    for( uint16_t i = 0, j ; i < net->layers ; i++ ) for( j = 0 ; j < net->neurons[i] ; j++ ) activate( &net->nn[i][j] );
    return *net->out;
}
