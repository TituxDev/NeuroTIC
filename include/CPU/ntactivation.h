/**
 * @file ntactivation.h
 * @author Oscar Sotomayor (Titux)
 * @brief @copybrief ntactivation.c
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
 * @ref ntactivation.c "Source code"
 */


#ifndef NTACTIVATION_H
#define NTACTIVATION_H

/**
 * @brief Identifiers for supported activation operations.
 * @details
 * Each identifier selects a row in the activation lookup tables defined
 * by this module.
 */
typedef enum {
    NTACT_BOOLEAN,   ///< Boolean step activation function
    NTACT_SIGMOID,   ///< Sigmoid activation function

    NTACT_TOTAL_FUNCTIONS ///< Total number of activation functions
} ntact_function_id_t;

/**
 * @details
 * This table maps each activation identifier to its corresponding execution
 * routines for both the activation function and its derivative.
 */
extern float (*ntact_activation[NTACT_TOTAL_FUNCTIONS][2])(float);


/**
 * @copydoc ntact_rand_range
 */
extern float ntact_rand_range[NTACT_TOTAL_FUNCTIONS][2];

#endif // NTACTIVATION_H