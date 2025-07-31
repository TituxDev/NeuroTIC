/**
 * @note This file is part of the NeuroTIC project.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file creation.h
 * @brief Header file for network creation functions in the NeuroTIC neural network framework.
 * @details
 * This header file declares functions for defining and building neural networks,
 * including memory management for neurons, inputs, and weights. It allows for dynamic
 * configuration of network topology and is designed to work with the `net` and
 * `neuron` structures defined in `neuroticore.h`. The functions handle memory
 * allocation, initialization, and linking of neurons, inputs, and outputs,
 * enabling the creation of flexible and efficient neural networks.
 */

#ifndef NTIC_CREATION_H
#define NTIC_CREATION_H

#include "NTIC_neuroticore.h"

/**
* @fn define_net
* @brief Defines a neural network structure with specified parameters.
* This function initializes a `net` structure with the given number of inputs,
* layers, and neurons per layer, preparing it for further configuration and memory allocation.
* @param inputs The total number of input signals expected by the network.
* @param layers The total number of neuron layers in the network.
* @param neurons_by_layer An array specifying the number of neurons in each layer.
* @return A `net` structure initialized with the specified parameters.
*/
struct net define_net(int inputs , int layers, int *neurons_by_layer );

/**
 * @fn build_net
 * @brief Builds a neural network structure based on the defined parameters.
 * This function allocates memory for the neurons, inputs, and outputs of the network,
 * linking them together according to the specified topology.
 * @param Net Pointer to a `net` structure that has been defined using `define_net`.
 * @return A pointer to the initialized `net` structure with all fields set up.
 */
struct net *build_net( struct net *Net );

#endif
