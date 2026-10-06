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
 * @brief Allocates the network's neuron and wiring structures.
 *
 * @param net Network with net_s::inputs and net_s::layers already set.
 * @param neurons_per_layer Array of per-layer neuron counts, size `layers_size`.
 * @param layers_size Must equal `net->layers`.
 * @return The same `net` pointer received, with net_s::neurons, net_s::nn and,
 *         when applicable, net_s::wiring allocated.
 */
struct net_s *newnet( net_s *net, uint16_t *neurons_per_layer, layer_t layers_size );

/**
 * @brief Defines the input count for every neuron.
 *
 * @param net Network with its neuron and wiring structures already allocated.
 * @return 1 if `net` is NULL; otherwise 0.
 */
uint8_t defineneurons( net_s *net );

/**
 * @brief Allocates the weight array for every neuron.
 *
 * @param net Network with neuron_s::inputs already defined.
 * @return 1 if `net` is NULL; otherwise 0.
 */
uint8_t buildneurons( net_s *net );

/**
 * @brief Resolves wiring descriptors and builds the network's internal references.
 *
 * @param net Network with neuron and wiring descriptors already defined.
 * @return The same `net` pointer, with its internal buffers, neuron inputs and
 *         weight arrays allocated and initialized.
 */
struct net_s *buildnet( net_s *net );

#endif // NTBUILDER_H