/**
 * ntcalculate.h - Neural network computation functions for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * Declares functions for calculating neuron weighted sums, applying activation
 * functions, and performing feedforward propagation through a neural network.
 * Designed for modular computation and easy extension to additional calculation
 * models.
 */

#ifndef NTCALCULATE_H
#define NTCALCULATE_H

#include "ntcore.h"

/**
 * @brief Calculate the weighted sum of a neuron’s inputs plus bias.
 *
 * Multiplies each input by its corresponding weight and sums the results,
 * adding the bias term.
 *
 * @param neuron Pointer to the neuron structure.
 * @return Weighted sum as a float.
 */
float weighing( neuron_t *neuron );

/**
 * @brief Apply the neuron’s activation function to the weighted sum.
 *
 * Computes the weighted sum and then applies the activation function
 * indexed by the neuron’s `fn` field. The output value is stored in the
 * neuron’s `out` field.
 *
 * @param neuron Pointer to the neuron structure.
 * @return Activated output value as a float.
 */
float activate( neuron_t *neuron );

/**
 * @brief Perform feedforward computation on the entire network.
 *
 * Iterates through all neurons in all layers, computing their outputs
 * by applying the activation function to weighted sums. Returns a pointer
 * to the output values of the final layer neurons.
 *
 * @param net Pointer to the neural network structure.
 * @return Pointer to the array of output neuron values (final layer).
 */
float *feedforward( net_t *net );

#endif // NTCALCULATE_H

/*
================================================================================
1. WEIGHTED SUM CALCULATION (weighing)
================================================================================

Calculates the sum of weighted inputs plus bias for a single neuron.

Example:

    float sum = weighing(&net.nn[0][0]);

================================================================================
2. ACTIVATION (activate)
================================================================================

Applies the neuron’s activation function to the weighted sum and stores
the output.

Example:

    float output = activate(&net.nn[0][0]);

================================================================================
3. FEEDFORWARD COMPUTATION (feedforward)
================================================================================

Computes outputs for all neurons layer-by-layer in feedforward order.

Example:

    float *outputs = feedforward(&net);

    // 'outputs' points to final layer neuron outputs.

================================================================================
4. NOTES
================================================================================

- Functions assume a properly constructed network (`net_t`) with
  initialized weights and inputs.
- `activate` updates the neuron’s `out` field for downstream use.
- `feedforward` returns a pointer to the final output layer’s output values.
*/
