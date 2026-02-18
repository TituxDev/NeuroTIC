/**
 * @file ntbuilder.h
 * @brief Network construction functions for the NeuroTIC framework.
 *
 * @copydetails ntbuilder.c
 * 
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntbuilder.c
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
 * @return net_s Pointer to the initialized network on success, or NULL on failure.
 */
#define NEWNET( network, neurons) newnet( &network, neurons, (layer_t)sizeof( neurons )/sizeof( uint16_t ) )

/**
 * @brief Convenience macro to create and build a feedforward network.
 *
 * Combines `newnet()` and `buildnet()` into a single step for creating
 * a feedforward network with the specified input size and layer configuration.
 *
 * @param network Name of the network variable to create.
 * @param i Number of external inputs to the network.
 * @param neurons Array defining the number of neurons in each layer.
 * @return net_s Pointer to the fully constructed feedforward network.
 */
#define CREATE_NET_FEEDFORWARD( network , i , neurons ) \
    net_s network={ \
        .inputs= i, \
        .layers= sizeof( neurons )/sizeof( uint16_t ), \
    }; \
    buildnet( newfeedforward( NEWNET( network , neurons ) ) );

/**
 * @brief Initializes a new neural network structure.
 *
 * @param net Pointer to the network structure to initialize.
 * @param neurons_per_layer Array specifying the number of neurons in each layer.
 * @param layers_size Number of layers (size of neurons_per_layer array).
 * @return Pointer to the initialized network on success, or NULL on failure.
 */
struct net_s *newnet (net_s *net , uint16_t *neurons_per_layer , layer_t layers_size );

/**
 * @brief Builds the internal buffers and connections of a neural network.
 *
 * @param net Pointer to the network structure to build.
 * @return Pointer to the fully constructed network.
 */
struct net_s *buildnet( net_s *net );

#endif // NTBUILDER_H