/**
 * @file ntcalculate.h
 * @brief Computation Module.  
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
 * @param net Pointer to the network.
 * @return Pointer to the output buffer of the final layer.
 */
data_t *feedforward( net_s *net );

#endif // NTCALCULATE_H
