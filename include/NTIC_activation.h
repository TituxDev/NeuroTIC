/**
 * @note This file is part of the NeuroTIC project.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file activation.h
 * @brief Activation functions for neurons in the NeuroTIC framework.
 * @details
 * This file declares the activation functions used by neurons in the NeuroTIC framework.
 * It includes function pointers for various activation functions such as sigmoid,
 * ReLU, and tanh, allowing for flexible activation behavior in neural networks.
 * The functions are designed to be compatible with the neuron data structure defined
 * in `neuroticore.h`.
 * Future revisions may include additional activation functions or macros for
 * common activation patterns.
 * This header only contains declarations and no function implementations.
 * Future revisions may include utility functions for activation function management.
 */

#ifndef TIC_ACTIVATION_H
#define TIC_ACTIVATION_H

/**
 * @enum functions
 * @brief Enumeration of available activation functions.
 * @details
 * This enumeration defines the available activation functions that can be used
 * within the NeuroTIC framework. Each function corresponds to a specific activation
 * behavior for neurons. The `TOTAL_FUNCTIONS` constant indicates the total number
 * of activation functions defined.
 * The functions are indexed for easy access in the `activation` array.
 * The `rand_range` array provides a range for random initialization of weights
 * or biases associated with each activation function.
 * The first index of the `activation` array corresponds to the function type,
 * while the second index indicates whether the function is for input or output.
 */
enum functions{
    BOOLEAN,
    SIGMOID,

    TOTAL_FUNCTIONS
};

/**
 * @var activation
 * @brief Array of function pointers for activation functions.
 * @details
 * This array holds pointers to the activation functions defined in the NeuroTIC framework.
 * Each function pointer corresponds to an activation function that can be applied to a neuron.
 * The first dimension of the array corresponds to the function type, while the second dimension
 * indicates whether the function is used for input or output.
 */
extern float ( *activation[TOTAL_FUNCTIONS][2] )( float );

/**
 * @var rand_range
 * @brief Array of ranges for random initialization of weights or biases.
 * @details
 * This array defines the range for random initialization of weights or biases associated
 * with each activation function. The first index corresponds to the function type, while the
 * second index indicates the minimum and maximum values for the range.
 */
extern int rand_range[TOTAL_FUNCTIONS][2];

#endif
