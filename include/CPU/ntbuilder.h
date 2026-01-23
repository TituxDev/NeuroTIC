/**
 * @file ntbuilder.h
 * @brief Network construction functions for the NeuroTIC framework.
 *
 * Provides utilities to create and build neural networks, allocating neurons,
 * buffers, and connections dynamically. Includes convenience macros to
 * simplify network creation.
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#ifndef NTBUILDER_H
#define NTBUILDER_H

#include "ntcore.h"
#include <stddef.h>

/**
 * @brief Convenience macro to create a new network.
 *
 * Simplifies calling `newnet()` by automatically calculating the number
 * of layers from the `neurons` array size.
 *
 * @param network Pointer to the network structure to initialize.
 * @param neurons Array defining the number of neurons per layer.
 */
#define NEWNET( network, neurons) newnet( &network, neurons, sizeof( neurons )/sizeof( uint16_t ) )

/**
 * @brief Initializes a new neural network structure.
 *
 * Allocates memory for the neurons and layers, and sets initial pointers
 * to NULL. Uses `memtrack` to register allocations for automatic cleanup.
 *
 * @param net Pointer to the network structure to initialize.
 * @param neurons_per_layer Array specifying the number of neurons in each layer.
 * @param layers_size Number of layers (size of neurons_per_layer array).
 * @return Pointer to the initialized network on success, or NULL on failure.
 */
struct net_s *newnet (net_s *net , uint16_t *neurons_per_layer , size_t layers_size );

/**
 * @brief Builds the internal buffers and connections of a neural network.
 *
 * Allocates memory for input/output pointers and inter-layer buffers
 * according to `bff_wiring`. Sets up neuron input references and weight arrays.
 *
 * @param net Pointer to the network structure to build.
 * @return Pointer to the fully constructed network.
 *
 * Buffer types:
 * - 'M': Mixed buffer with individual connections determined by `src_type`.
 * - 'N': Buffer shared with another buffer in the network.
 * - 'I': References network input array.
 * - 'O': References network output array.
 */
struct net_s *buildnet( net_s *net );

#endif // NTBUILDER_H