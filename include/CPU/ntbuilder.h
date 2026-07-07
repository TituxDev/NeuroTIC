/**
 * @file ntbuilder.h
 * @copybrief ntbuilder.c
 * 
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntbuilder.c
 *
 * @copydetails ntbuilder.c
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
#define NEWNET( network, neurons) newnet( network, neurons, (layer_t)sizeof( neurons )/sizeof( uint16_t ) )

/**
 * @brief Convenience macro to declare, build, and wire a new feedforward
 *        network in a single step.
 *
 * Expands to a variable declaration followed by its construction: declares
 * `network` as a new `net_s *` in the calling scope, then builds and wires
 * it as a fully-connected feedforward network. Because it expands to a
 * declaration, it can only be used as a standalone statement -- never as
 * part of a larger expression.
 *
 * @param network Identifier to declare as the new network variable.
 * @param i Number of external inputs to the network.
 * @param neurons Array defining the number of neurons in each layer.
 */
#define CREATE_NET_FEEDFORWARD( network , i , neurons ) \
    net_s *network= &(net_s){ \
        .inputs= i, \
        .layers= sizeof( neurons )/sizeof( uint16_t ), \
    }; \
    buildnet( newfeedforward( NEWNET( network , neurons ) ) );

/**
 * @brief Allocates and initializes net_s internal structures from a per-layer neuron count.
 *
 * @param net Network with net_s::inputs and net_s::layers already set.
 * @param neurons_per_layer Array of per-layer neuron counts, size `layers_size`.
 * @param layers_size Must equal `net->layers`.
 * @return The same net pointer received, with its internal structures
 *         allocated and initialized.
 */
struct net_s *newnet (net_s *net , uint16_t *neurons_per_layer , layer_t layers_size );

/**
 * @brief Resolves wiring descriptors into buffer references and allocates weight storage.
 *
 * @param net Network with wiring descriptors already defined.
 * @return The same `net` pointer, for call chaining.
 */
struct net_s *buildnet( net_s *net );

#endif // NTBUILDER_H