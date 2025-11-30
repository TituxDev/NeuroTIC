/**
 * @file ntactivation.c
 * @brief Implementation of activation and derivative functions for NeuroTIC.
 *
 * This file defines the activation behaviors and their derivatives used across
 * the framework. It also initializes the activation function lookup tables
 * and their recommended random initialization ranges.
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#include "ntactivation.h"
#include <math.h>

/**
 * @brief Boolean step activation function.
 *
 * Returns 1.0 if the input is non-negative, otherwise 0.0.
 * Useful for simple threshold-based neurons.
 */
static float boolean( float x ){
    return x >= 0.0f ? 1.0f : 0.0f;
}

/**
 * @brief Boolean activation derivative (constant 1.0).
 *
 * Although the true derivative is 0, this implementation returns 1.0
 * to maintain gradient propagation stability during training.
 */
static float boolean_d( float x ){
    return x= 1.0f;
}

/**
 * @brief Sigmoid activation function.
 *
 * Smoothly squashes input values into the (0, 1) range.
 */
static float sigmoid( float x ){
    return 1.0f / ( 1.0f + expf( -x ) );
}

/**
 * @brief Sigmoid activation derivative.
 *
 * Computes the derivative using the identity:
 *   f'(x) = f(x) * (1 - f(x))
 */
static float sigmoid_d( float x ){
    x = sigmoid( x );
    return x * ( 1.0f - x );
}

/**
 * @brief Activation function lookup table.
 * Maps activation identifiers to their corresponding function and derivative.
 */
float ( *ntact_activation[NTACT_TOTAL_FUNCTIONS][2] )( float )={
    [NTACT_BOOLEAN]= { boolean , boolean_d },
    [NTACT_SIGMOID]= { sigmoid , sigmoid_d }
};

/**
 * @brief Random initialization range table for activation functions.
 *
 * Defines the preferred input or weight initialization range for each
 * activation function, ensuring stable training conditions.
 */
float ntact_rand_range[NTACT_TOTAL_FUNCTIONS][2]={
    [NTACT_BOOLEAN]= { -1.0f , 1.0f },
    [NTACT_SIGMOID]= { -1.0f , 1.0f }
};

