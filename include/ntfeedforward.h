/**
 * @file ntfeedforward.h
 * @brief Predefined feedforward topologies for NeuroTIC networks.
 *
 * Provides convenient functions to initialize common neural network wiring
 * configurations. These functions allocate and configure the network's
 * `bff_wiring` structure according to a specific topology.
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#ifndef NTFEEDFORWARD_H
#define NTFEEDFORWARD_H

#include "ntcore.h"

/**
 * @brief Initializes a simple feedforward topology.
 *
 * Sets up one buffer per neuron connecting each neuron to the corresponding
 * neuron in the previous layer.
 *
 * @param net Pointer to the network to configure.
 * @return Pointer to the configured network, or NULL if input is invalid.
 */
struct net_s *newfeedforward( net_s *net );

/**
 * @brief Initializes a dense feedforward topology.
 *
 * Connects each neuron in a layer to all neurons in previous layers,
 * creating a fully connected inter-layer mapping.
 *
 * @param net Pointer to the network to configure.
 * @return Pointer to the configured network, or NULL if input is invalid.
 */
struct net_s *newdense( net_s *net );

#endif // NTFEEDFORWARD_H