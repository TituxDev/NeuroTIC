/**
 * @file ntactivation.h
 * @copybrief ntactivation.c
 *   
 * @ref http://tituxdev.github.io/NeuroTIC/src/CPU/ntactivation.c
 * 
 * @copydetails ntactivation.c
 */


#ifndef NTACTIVATION_H
#define NTACTIVATION_H

/**
 * @brief Enumeration of supported activation functions.
 * 
 * @details
 * This enumeration defines identifiers for the activation functions implemented in the NeuroTIC framework.  
 * Each identifier corresponds to a specific activation function and its derivative, which are stored in the `ntact_activation` dispatch table.  
 * The `NTACT_TOTAL_FUNCTIONS` entry is used to indicate the total number of activation functions available, which is useful for iterating over the dispatch table and for validating activation function identifiers.
 *
 * @note This set is not fixed -- additional activation functions can be
 * added as the framework grows.
 */
typedef enum {
    NTACT_BOOLEAN,   ///< Boolean step activation function
    NTACT_SIGMOID,   ///< Sigmoid activation function
    NTACT_TANH,      ///< Hyperbolic tangent activation function
    NTACT_RELU,      ///< Rectified Linear Unit activation function
    NTACT_LRELU,     ///< Leaky Rectified Linear Unit activation function

    NTACT_TOTAL_FUNCTIONS ///< Total number of activation functions
} ntact_function_id_t;

/**
 * @brief Activation function and derivative dispatch tables.
 */
extern float (*ntact_activation[NTACT_TOTAL_FUNCTIONS][2])( float );


/**
 * @brief Random initialization range table for activation functions.
 */
extern float ntact_rand_range[NTACT_TOTAL_FUNCTIONS][2];

#endif // NTACTIVATION_H