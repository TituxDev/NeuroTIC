/**
 * @file ntfile.h
 * @copybrief ntfile.c
 * 
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntfile.c
 *
 * @copydetails ntfile.c
 * 
 */

#ifndef NTFILE_H
#define NTFILE_H

#include <stddef.h>
#include "ntcore.h"

/**
 * @brief Saves a network to a binary file with extension .ntic
 * 
 * @param net Pointer to the network to save.
 * @param name Base filename (without extension) to save the network as.
 * @return The size, in bytes, of the file written.
 */
size_t savenet( net_s * net , const char *name );

/**
 * @brief Loads a network from a binary file with extension .ntic
 * 
 * @param net Pointer to a net_s instance to populate with the loaded network.
 * @param name Base filename (without extension) to load the network from.
 * @return 0 on success.
 */
size_t loadnet( net_s *net , const char *name );

#endif // NTFILE_H