/**
 * @file ntfile.h
 * @brief Save and Load Files.  
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntfile.c
 *
 * @copydetails ntfile.c
 * 
 */

#ifndef NTFILE_H
#define NTFILE_H

#include "ntcore.h"

/**
 * @brief Saves a network to a binary file with extension .ntic
 * 
 * @param net Pointer to the network to save.
 * @param name Base filename (without extension) to save the network as.
 * @return 0 on success, non-zero on failure.
 */
unsigned char savenet( net_s * net , const char *name );

/**
 * @brief Loads a network from a binary file with extension .ntic
 * 
 * @param name Base filename (without extension) to load the network from.
 * @return The loaded network structure. If loading fails, returns an empty network.
 */
struct net_s loadnet( char *name );

#endif // NTFILE_H