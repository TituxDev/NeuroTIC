/**
 * ntactivation.c - Implementation of activation functions for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * Contains core activation functions and their derivatives used in the NeuroTIC framework.
 * Requires linking with the math library (-lm).
 */

#include "ntactivation.h"
#include <math.h>

/**
 * @brief Boolean step activation function.
 * Returns 1.0f if x >= 0, else 0.0f.
 */
static float boolean( float x ){
    return x >= 0.0f ? 1.0f : 0.0f;
}

/**
 * @brief Derivative of boolean step function.
 * The derivative of a step function is zero almost everywhere; 
 * here, it returns 1.0f for simplicity in backpropagation placeholders.
 */
static float boolean_d( float x ){
    return x= 1.0f;
}

/**
 * @brief Sigmoid activation function.
 * Returns the sigmoid of x.
 */
static float sigmoid( float x ){
    return 1.0f / ( 1.0f + expf( -x ) );
}

/**
 * @brief Derivative of sigmoid function.
 * Uses the property: sigmoid'(x) = sigmoid(x) * (1 - sigmoid(x))
 */
static float sigmoid_d( float x ){
    x = sigmoid( x );
    return x * ( 1.0f - x );
}

/**
 * @brief Table of activation functions and their derivatives.
 *
 * Indexed by [function_id][0] for function and [function_id][1] for derivative.
 */
float ( *ntact_activation[NTACT_TOTAL_FUNCTIONS][2] )( float )={
    [NTACT_BOOLEAN]= { boolean , boolean_d },
    [NTACT_SIGMOID]= { sigmoid , sigmoid_d }
};

/**
 * @brief Recommended random initialization ranges for weights per function.
 *
 * Used when initializing neurons to stabilize learning.
 */
float ntact_rand_range[NTACT_TOTAL_FUNCTIONS][2]={
    [NTACT_BOOLEAN]= { -1.0f , 1.0f },
    [NTACT_SIGMOID]= { -1.0f , 1.0f }
};

