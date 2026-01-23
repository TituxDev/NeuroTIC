/**
 * @file ntactivation.h
 * @brief Activation function interface for the NeuroTIC framework.
 *
 * This module defines the available activation functions and provides global
 * references to their implementations and derivative counterparts. Each neuron
 * in the network selects an activation through its function index (`fn`).
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#ifndef NTACTIVATION_H
#define NTACTIVATION_H

/**
 * @enum ntact_function_id_t
 * @brief Identifiers for the supported activation functions.
 *
 * Each identifier maps to a pair of function pointers defined in the
 * `ntact_activation` matrix, where column 0 is the activation function
 * and column 1 is its derivative.
 */
typedef enum {
    NTACT_BOOLEAN,   ///< Boolean step activation function
    NTACT_SIGMOID,   ///< Sigmoid activation function

    NTACT_TOTAL_FUNCTIONS ///< Total number of activation functions
} ntact_function_id_t;

/**
 * @brief Activation function lookup table.
 * Each row corresponds to an activation function listed in
 * `ntact_function_id_t`, and the two columns contain pointers to:
 * - `[0]`: the activation function itself.
 * - `[1]`: its first derivative.
 */
extern float (*ntact_activation[NTACT_TOTAL_FUNCTIONS][2])(float);


/**
 * @brief Recommended random initialization ranges for each activation function.
 *
 * Each row corresponds to a function listed in `ntact_function_id_t`, defining
 * the lower and upper range limits for initializing network weights.
 */
extern float ntact_rand_range[NTACT_TOTAL_FUNCTIONS][2];

#endif // NTACTIVATION_H