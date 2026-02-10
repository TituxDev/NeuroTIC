/**
 * @file ntactivation.h
 * @ingroup NTMath
 * @brief Activation function interface.
 * @author Oscar Sotomayor (Titux)
 *
 * @details
 * This module defines the set of activation operations supported by the system
 * and exposes global lookup tables that bind activation identifiers to their
 * corresponding execution routines and derivatives.
 *
 * Although this interface is publicly accessible, it is typically consumed
 * indirectly through higher-level execution components.
 *
 * The activation subsystem itself is stateless and purely functional.
 *
 */


#ifndef NTACTIVATION_H
#define NTACTIVATION_H

/**
 * @enum ntact_function_id_t
 * @brief Identifiers for supported activation operations.
 *
 * Each identifier selects a row in the activation lookup tables defined
 * by this module.
 */
typedef enum {
    NTACT_BOOLEAN,   ///< Boolean step activation function
    NTACT_SIGMOID,   ///< Sigmoid activation function

    NTACT_TOTAL_FUNCTIONS ///< Total number of activation functions
} ntact_function_id_t;

/**
 * @brief Activation execution dispatch table.
 *
 * This table binds activation identifiers to their corresponding execution
 * routines.
 *
 * Conceptual layout:
 *
 * @verbatim
 *                +-------------------+
 *                | ntact_activation  |
 *                +-------------------+
 *                | [fn][0] -> f(x)   |
 *                | [fn][1] -> f'(x)  |
 *                +-------------------+
 *
 *   fn ∈ ntact_function_id_t
 * @endverbatim
 *
 * Column semantics:
 * - [0] Activation function
 * - [1] First derivative
 *
 * This table is typically accessed by execution subsystems rather than
 * user code.
 */
extern float (*ntact_activation[NTACT_TOTAL_FUNCTIONS][2])(float);


/**
 * @brief Recommended initialization ranges per activation.
 *
 * This table provides suggested numeric ranges associated with each
 * activation operation.
 *
 * Conceptual layout:
 *
 * @verbatim
 *   ntact_rand_range[fn] = { min , max }
 *
 *   fn ∈ ntact_function_id_t
 * @endverbatim
 *
 * These values are advisory and are typically consumed by internal
 * initialization logic.
 */
extern float ntact_rand_range[NTACT_TOTAL_FUNCTIONS][2];

#endif // NTACTIVATION_H