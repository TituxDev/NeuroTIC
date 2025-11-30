/**
 * @file ntinitialize.h
 * @brief Random weight initialization for NeuroTIC networks.
 *
 * Provides utilities to initialize network weights and biases prior to training.
 * Weights are set randomly within the range defined by the activation function,
 * and biases are set to zero.
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#ifndef NTINITIALIZE_H
#define NTINITIALIZE_H

#include "ntcore.h"

/**
 * @brief Randomly initializes network weights and biases.
 *
 * Each neuron weight is set randomly within the limits specified by the
 * corresponding activation function (`ntact_rand_range`). Biases are initialized to 0.
 *
 * @param net Pointer to the network to initialize.
 */
void randnet( net_s *net);

#endif // NTINITIALIZE_H