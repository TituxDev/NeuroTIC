/**
 * @note This file is part of the NeuroTIC project.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file initialize.h
 * @brief Header file for initializing neural networks in the NeuroTIC framework.
 * @details
 * This header file declares the `rand_net` function, which initializes the weights and biases
 * of the neurons in a neural network with random values. The function uses the `rand_range` array
 * defined in `activation.h` to determine the range of random values for each activation function.
 * It is designed to work with the `net` and `neuron` structures defined in `neuroticore.h`.
 * The function is part of the larger NeuroTIC project, which provides a framework for building and training neural networks.
 * The implementation is modular, allowing for easy extension with additional features or configurations in the future.
 * The header file includes necessary definitions and declarations
 * for the NeuroTIC framework, ensuring that the `rand_net` function can be used
 * in conjunction with other components of the framework.
 * @note This file is part of the NeuroTIC project, which is a neural network library.
 * It provides a flexible and efficient way to initialize neural networks,
 * allowing for dynamic configuration of network topology and parameters.
 * The `rand_net` function is essential for preparing the network for training or inference,
 * ensuring that all neurons have properly initialized weights and biases.
 * @warning Ensure that the `net` structure is properly defined before calling `rand_net`.
 * If the structure is not properly defined, it may lead to undefined behavior or memory corruption.
 * The function assumes that the `net` structure has been defined with valid parameters
 * before calling this function. If the structure is not properly defined,
 * it may lead to undefined behavior or memory corruption.
 * @note This header file is intended to be included in source files that require
 * the initialization of neural networks. It provides the necessary declarations
 * and definitions to ensure that the `rand_net` function can be used effectively
 * within the NeuroTIC framework.
 * The `rand_net` function is designed to be called after the network structure has been defined
 * using the `define_net` function, ensuring that all necessary parameters are set up correctly.
 * The function initializes the weights and biases of the neurons in the network,
 * preparing the network for training or inference.
 * @warning Ensure that the `net` structure has been defined using `define_net`
 * before calling this function. If the structure is not properly defined,
 * it may lead to undefined behavior or memory corruption.
 * The function does not perform any validation on the `net` structure,
 * so it is the caller's responsibility to ensure that the structure is correctly defined.
 * If the structure is not properly defined, it may lead to undefined behavior or memory corruption.
 * The function assumes that the `net` structure has been defined with valid parameters
 * before calling this function. If the structure is not properly defined,
 * it may lead to undefined behavior or memory corruption.
 * The function initializes the weights and biases of the neurons in the network,
 * preparing the network for training or inference.
 * The function uses the `rand_range` array defined in `activation.h` to determine
 * the range of random values for each activation function, ensuring that the
 * initialization is consistent with the activation functions used in the network.
 * The function is designed to be called after the network structure has been defined
 * using the `define_net` function, ensuring that all necessary parameters are set up correctly.
 * The function initializes the weights and biases of the neurons in the network,
 * preparing the network for training or inference.
 * The function is part of the larger NeuroTIC project, which provides a framework
 * for building and training neural networks. It is designed to be modular and extensible,
 * allowing for easy integration with other components of the framework.
 * The function is intended to be used in conjunction with the `neuroticore.h`
 * header file, which defines the `net` and `neuron` structures used in the
 * NeuroTIC framework.
 * The function is designed to be called after the network structure has been defined
 * using the `define_net` function, ensuring that all necessary parameters are set up correctly.
 * The function initializes the weights and biases of the neurons in the network,
 * preparing the network for training or inference.
 * The function is part of the larger NeuroTIC project, which provides a framework
 * for building and training neural networks. It is designed to be modular and extensible,
 * allowing for easy integration with other components of the framework.
 * The function is intended to be used in conjunction with the `neuroticore.h`
 * header file, which defines the `net` and `neuron` structures used in the
 * NeuroTIC framework.
 * The function is designed to be called after the network structure has been defined
 * using the `define_net` function, ensuring that all necessary parameters are set up correctly.
 * The function initializes the weights and biases of the neurons in the network,
 * preparing the network for training or inference.
 * The function is part of the larger NeuroTIC project, which provides a framework
 * for building and training neural networks. It is designed to be modular and extensible,
 * allowing for easy integration with other components of the framework.
 * The function is intended to be used in conjunction with the `neuroticore.h`
 * header file, which defines the `net` and `neuron` structures used in the
 * NeuroTIC framework.
 * The function is designed to be called after the network structure has been defined
 * using the `define_net` function, ensuring that all necessary parameters are set up correctly.
 */

#ifndef NTIC_INITIALIZE_H
#define NTIC_INITIALIZE_H

#include "NTIC_neuroticore.h"

/**
 * @fn rand_net
 * @brief Initializes the weights and biases of the neural network with random values.
 * @param Net Pointer to the neural network structure to be initialized.
 * @details
 * This function initializes the weights and biases of the neurons in the neural network
 * with random values. It uses the `rand_range` array defined in `activation.h`
 * to determine the range of random values for each activation function.
 * The function iterates through all layers and neurons, setting the weights and biases accordingly.
 * The random number generation is seeded with the current time to ensure different results on each run.
 * The function is designed to be called after the network structure has been defined
 * using the `define_net` function, ensuring that all necessary parameters are set up correctly.
 * The function is part of the larger NeuroTIC project, which provides a framework
 * for building and training neural networks. It is designed to be modular and extensible,
 * allowing for easy integration with other components of the framework.
 * The function is intended to be used in conjunction with the `neuroticore.h`
 * header file, which defines the `net` and `neuron` structures used in the
 * NeuroTIC framework.
 * @warning Ensure that the `net` structure has been defined using `define_net`
 * before calling this function. If the structure is not properly defined,
 * it may lead to undefined behavior or memory corruption.
 */
void rand_net(struct net *Net);

#endif // NTIC_INITIALIZE_H