/**
 * @note This file is part of the NeuroTIC project.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @brief NeuroTIC is a neural network framework designed for flexibility and extensibility.
 * @details
 * The NeuroTIC framework provides a set of core data structures and functions for building,
 * training, and running neural networks. It is designed to be modular and extensible,
 * allowing users to define custom neurons, activation functions, and network architectures.
 * The framework supports various neural network types and is suitable for both educational
 * and research purposes. It is implemented in C++17 and follows modern C++ practices.
 * The framework is open-source and licensed under the Mozilla Public License v. 2.0
 * to encourage collaboration and contributions from the community.
 * Future revisions may include additional features such as support for convolutional
 * and recurrent neural networks, improved training algorithms, and enhanced performance optimizations.
 * The NeuroTIC project aims to provide a comprehensive and user-friendly framework for
 * neural network development, making it accessible to both beginners and experienced developers.
 * The project is actively maintained, and contributions are welcome.
 * For more information, please visit the project's repository or documentation.
 */

#ifndef NEUROTIC_H
#define NEUROTIC_H

/**
 * @file neuroticore.h
 * @brief Core data structures for the NeuroTIC neural network framework.
 * @details
 * This file defines the core data structures used in the NeuroTIC framework.
 * It includes the neuron structure, which contains information about the neuron's
 * activation function, weights, biases, and other relevant parameters.
 * The neuron structure is designed to be flexible and extensible, allowing for
 * various types of neural networks to be implemented.
 * Future revisions may include additional data structures for different types of
 * neural networks or utility functions for managing neurons.
 * This header only contains declarations and no function implementations.
 */
#include "NTIC_neuroticore.h"

/**
 * @file activation.h
 * @brief Activation functions for the NeuroTIC neural network framework.
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
#include "NTIC_activation.h"

/**
 * @file creation.h
 * @brief Header file for network creation functions in the NeuroTIC neural network framework.
 * @details
 * This header file declares functions for defining and building neural networks,
 * including memory management for neurons, inputs, and weights. It allows for dynamic
 * configuration of network topology and is designed to work with the `net` and
 * `neuron` structures defined in `neuroticore.h`. The functions handle memory
 * allocation, initialization, and linking of neurons, inputs, and outputs,
 * enabling the creation of flexible and efficient neural networks.
 */
#include "NTIC_creation.h"

/**
 * @file initialize.h
 * @brief Header file for initializing neural networks in the NeuroTIC framework.
 * @details
 * This header file declares the `rand_net` function, which initializes the weights and biases
 * of the neurons in a neural network with random values. The function uses the `rand_range` array
 * defined in `activation.h` to determine the range of random values for each activation function.
 * It is designed to work with the `net` and `neuron` structures defined in `neuroticore.h`.
 * The function is part of the larger NeuroTIC project, which provides a framework for building and training neural networks.
 */
#include "NTIC_initialize.h"

/**
 * @file calculate.h
 * @brief Header file for neural network calculations.
 * @details
 * This header file declares functions for calculating the output of neurons and running
 * the neural network. It includes the `weighing` function that computes the weighted sum
 * of inputs, the `evaluate` function that applies the activation function to the weighted sum,
 * and the `run_net` function that processes the entire network by evaluating each neuron in each
 * layer. The functions are designed to work with the neuron and net structures defined in
 * `NTIC_neuroticore.h` and the activation functions declared in `NTIC_activation.h`.
 */
#include "NTIC_calculate.h"

/**
 * @file train.h
 * @brief Header file for training functions in the NeuroTIC neural network framework.
 * @details
 * This header file declares the `train` function that implements the backpropagation algorithm
 * to adjust the weights and biases of the neural network based on input samples and expected results.
 * The training process iterates through the provided samples, calculates the error,
 * and updates the weights using the specified learning rate until the error is below a defined tolerance
 * or the maximum number of attempts is reached.
 */
#include "NTIC_train.h"

#endif // NTIC_NEUROTIC_H