/**
 * @file ntinitialize.h
 * @copybrief ntinitialize.c
 * 
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntinitialize.c
 * 
 * @copydetails ntinitialize.c
 */

#ifndef NTINITIALIZE_H
#define NTINITIALIZE_H

#include "ntcore.h"

/**
 * @brief Randomly initializes network weights and resets biases to zero.
 *
 * @param net Pointer to a net_s instance whose base structure has already
 *            been built.
 */
void randnet( net_s *net);

#endif // NTINITIALIZE_H