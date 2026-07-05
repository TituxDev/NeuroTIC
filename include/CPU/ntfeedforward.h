/**
 * @file ntfeedforward.h
 * @copybrief ntfeedforward.c
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntfeedforward.c
 *
 * @copydetails ntfeedforward.c
 */

#ifndef NTFEEDFORWARD_H
#define NTFEEDFORWARD_H

#include "ntcore.h"

/**
 * @brief Wires each layer as fully connected to only its immediately
 *        preceding layer.
 *
 * @param net Pointer to a net_s instance whose base structure has already
 *            been built.
 * @return The same net pointer received, with wiring and bff_idx set for
 *         every layer boundary.
 */
struct net_s *newfeedforward( net_s *net );

/**
 * @brief Wires each layer as fully connected to every preceding layer,
 *        cumulatively.
 *
 * @param net Pointer to a net_s instance whose base structure has already
 *            been built.
 * @return The same net pointer received, with wiring and bff_idx set for
 *         every layer boundary.
 */
struct net_s *newdense( net_s *net );

#endif // NTFEEDFORWARD_H