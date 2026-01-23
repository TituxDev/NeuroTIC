/**
 * @file ntfile.h
 * @brief Save and load NeuroTIC networks to/from binary files.
 *
 * Provides functions to persist network structures and weights,
 * allowing training to be stored and reloaded.
 *
 * @author Oscar Sotomayor
 * @date 2024
 */

#ifndef NTFILE_H
#define NTFILE_H

#include "ntcore.h"

/**
 * @brief Saves a network to a binary file with extension .ntic
 *
 * Stores the network's layers, neurons, weights, biases, and buffer wiring.
 *
 * @param net Pointer to the network to save.
 * @param name Base filename (without extension) to save the network.
 * @return 0 on success, non-zero on failure (future validation to be added).
 */
unsigned char savenet( net_s * net , const char *name );

/**
 * @brief Loads a network from a binary file with extension .ntic
 *
 * Reconstructs the network structure, weights, biases, and buffer wiring.
 * Currently, data standardization and validation are not integrated.
 *
 * @param name Base filename (without extension) of the saved network.
 * @return The loaded network structure.
 */
struct net_s loadnet( char *name );

#endif // NTFILE_H