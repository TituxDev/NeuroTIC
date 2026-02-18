/**
 * @file ntinitialize.h
 * @brief Random Initialization.  
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntinitialize.c
 * 
 * @copydetails ntinitialize.c
 */

#ifndef NTINITIALIZE_H
#define NTINITIALIZE_H

#include "ntcore.h"

/**
 * @brief Randomly initializes network weights.
 *
 * @param net Pointer to the network to initialize.
 */
void randnet( net_s *net);

#endif // NTINITIALIZE_H