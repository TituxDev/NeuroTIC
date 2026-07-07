/**
 * @file ntdefinition.h
 * @copybrief ntdefinition.c
 * 
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntdefinition.c
 *
 * @copydetails ntdefinition.c
 */


#ifndef NTDEFINITION_H
#define NTDEFINITION_H

#include "ntcore.h"

/**
 * @brief Interactively collects a network's structural parameters (inputs,
 *        layers, neurons per layer) via stdin and builds the base structure.
 * @param net Pointer to an allocated, uninitialized net_s instance to populate.
 * @return The same net pointer received, with its base structure built.
 */
net_s *definestructure( net_s *net );

/**
 * @brief Interactively collects, layer by layer, the wiring topology and
 *        each neuron's buffer selection, then builds the network.
 * @param net Pointer to a net_s instance already populated by definestructure().
 * @return The same net pointer received, fully built and ready to evaluate.
 */
net_s *definetopology( net_s *net );

#endif /* NTDEFINITION_H */