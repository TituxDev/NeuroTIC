/**
 * @note This file is part of the NeuroTIC project.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file activation.c
 * @brief Implementation of activation functions for the NeuroTIC neural network framework.
 * @details
 * This file provides implementations of various activation functions used in artificial neurons,
 * including Boolean step and sigmoid functions, along with their derivatives.
 * These functions are used to compute the output of neurons based on their inputs and weights.
 * The functions are designed to be used in conjunction with the `neuron` structure defined in
 * `neuroticore.h`, allowing for flexible and efficient evaluation, training, and inference
 * in neural networks.
 * The activation functions are accessed via an array of function pointers,
 * enabling easy selection and application of different activation strategies.
 * The implementation is designed to be modular, allowing for easy extension with additional activation functions in the future.
 */

#include "NTIC_activation.h"

#include <math.h>

/**
 * @brief Step activation function (Boolean threshold).
 *
 * Returns 1 if the input is greater or equal to zero, 0 otherwise.
 * This is a hard threshold activation commonly used in simple binary classifiers.
 *
 * @param x Input value to the activation function.
 * @return 1.0 if x >= 0, else 0.0.
 */
float boolean( float x ){
    return x >= 0;
}

/**
 * @brief Derivative of the Boolean step activation function.
 *
 * This derivative is constant 1 here for implementation convenience,
 * but note that the true derivative of the step function is zero almost everywhere except at 0.
 *
 * @param x Input value (ignored in this implementation).
 * @return Constant derivative value 1.0.
 */
float boolean_d( float x ){
    return x= 1;
}

/**
 * @brief Sigmoid activation function.
 *
 * Computes the sigmoid of the input value, which maps the input to a range
 * between 0 and 1. This function is commonly used in binary classification tasks.
 *
 * @param x Input value to the activation function.
 * @return Sigmoid of the input value.
 */
float sigmoid( float x ){
    return 1 / ( 1 + exp( -x ) );
}

/**
 * @brief Derivative of the sigmoid activation function.
 *
 * Computes the derivative of the sigmoid function, which is used during backpropagation
 * in neural networks. The derivative is calculated as sigmoid(x) * (1 - sigmoid(x)).
 *
 * @param x Input value to the activation function.
 * @return Derivative of the sigmoid function at x.
 */
float sigmoid_d( float x ){
    x= sigmoid( x );
    return x * ( 1 - x );
}

/**
 * @brief Array of activation function pointers.
 *
 * This array provides access to the various activation functions and their
 * derivatives defined in this module. Each entry corresponds to a specific
 * activation function, with the first element being the function itself and
 * the second element being its derivative.
 */
float ( *activation[TOTAL_FUNCTIONS][2] )( float )={
    [BOOLEAN]= { boolean , boolean_d },
    [SIGMOID]= { sigmoid , sigmoid_d }
};

/**
 * @brief Range of random values for each activation function.
 *
 * This array defines the range of random values that can be used with each
 * activation function. It is useful for initializing weights or biases in
 * neural networks.
 */
int rand_range[TOTAL_FUNCTIONS][2]={
    [BOOLEAN]= { -1 , 1 },
    [SIGMOID]= { -1 , 1 }
};
