/**
 * @file ntcalculate.h
 * @brief Core feedforward computations for NeuroTIC networks.
 *
 * Provides functions to perform weighted summation, activation, and full
 * forward propagation of a neural network in sequential order.
 *
 * @author Oscar Sotomayor
 * @date 2024
 */

#ifndef NTCALCULATE_H
#define NTCALCULATE_H

#include "ntcore.h"

/**
 * @brief Computes the weighted sum of a neuron's inputs plus bias.
 *
 * @param neuron Pointer to the neuron to compute.
 * @return The weighted sum (float).
 */
float weighing( neuron_s *neuron );

/**
 * @brief Applies the activation function to a neuron's weighted input.
 *
 * Stores the result in the neuron's `out` field.
 *
 * @param neuron Pointer to the neuron to activate.
 * @return The activated output value (float).
 */
float activate( neuron_s *neuron );

/**
 * @brief Performs full feedforward computation for a network.
 *
 * Iterates over all layers and neurons in sequential order, computing
 * activations for each neuron.
 *
 * @param net Pointer to the network to compute.
 * @return Pointer to the array of output values from the final layer.
 */
float *feedforward( net_s *net );

#endif // NTCALCULATE_H
