/**
 * @file ntcalculate.h
 * @copybrief ntcalculate.c
 *   
 * @ref https://tituxdev.github.io/NeuroTIC/src/CPU/ntcalculate.c
 * 
 * @copydetails ntcalculate.c
 */

#ifndef NTCALCULATE_H
#define NTCALCULATE_H

#include "ntcore.h"

/**
 * @brief Computes the weighted sum of a neuron.
 *
 * @param neuron Pointer to the neuron.
 * @return Weighted sum including bias.
 */
data_t weighing( neuron_s *neuron );

/**
 * @brief Applies the configured activation function.
 *
 * @param neuron Pointer to the neuron.
 * @return Activated output value.
 */
data_t activate( neuron_s *neuron );

/**
 * @brief Executes full feedforward propagation.
 *
 * @param net Pointer to a net_s instance whose base structure has already
 *            been built.
 * @return The network's own output array (net_s::out), with every
 *         neuron's output freshly computed.
 * 
 * @todo `net_s::in` is not validated before use -- if every external input
 * has been set to `NULL` (e.g. after training releases its own input
 * buffer), this function will dereference a NULL pointer instead of
 * failing predictably. Add a check for every `net_s::in[i]` before
 * computing.
 */
data_t **feedforward( net_s *net );

#endif // NTCALCULATE_H