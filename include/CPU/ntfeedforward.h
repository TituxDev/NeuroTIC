/**
 * @file ntfeedforward.h
 * @brief Predefined feedforward topologies for NeuroTIC networks.
 *
 * @copydetails ntfeedforward.c
 * 
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntfeedforward.c
 */

#ifndef NTFEEDFORWARD_H
#define NTFEEDFORWARD_H

#include "ntcore.h"

/**
 * @brief Initializes a simple feedforward topology.
 * 
 * @param net Pointer to the network to configure.
 * @return Pointer to the configured network, or NULL on invalid input.
 */
struct net_s *newfeedforward( net_s *net );

/**
 * @brief Initializes a dense feedforward topology.
 * 
 * @param net Pointer to the network to configure.
 * @return Pointer to the configured network, or NULL on invalid input.
 */
struct net_s *newdense( net_s *net );

#endif // NTFEEDFORWARD_H