/**
 * @note This file is part of NeuroTIC, a neural network library.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file train.h
 * @brief Header file for training functions in the neural network.
 * @details
 * This header file declares the `train` function that implements the backpropagation algorithm
 * to adjust the weights and biases of the neural network based on input samples and expected results.
 * The training process iterates through the provided samples, calculates the error,
 * and updates the weights using the specified learning rate until the error is below a defined tolerance
 * or the maximum number of attempts is reached.
 */

#ifndef NTIC_TRAIN_H
#define NTIC_TRAIN_H

#include "NTIC_neuroticore.h"

/**
 * @fn train
 * @brief Trains the neural network using backpropagation.
 * @param Net The network structure to be trained.
 * @param inputs Array of input samples for training.
 * @param results Array of expected output results corresponding to the inputs.
 * @param samples Number of input-output pairs in the training data.
 * @param learning_rate The rate at which the network learns during training.
 * @param tolerance The acceptable error threshold for training completion.
 * @param max_attempts Maximum number of training iterations allowed.
 * @return The number of attempts made to train the network before reaching the tolerance or max attempts.
 * @details
 * This function implements the backpropagation algorithm to adjust the weights and biases of the neural network.
 * It processes each input sample, calculates the output, computes the error, and updates the weights accordingly.
 * The function continues until either the total error is below the specified tolerance or the maximum number of attempts is reached.
 */
int train( struct net *Net , float **inputs , float **results , int samples , float learning_rate , float tolerance , int max_attempts );

#endif // NTIC_TRAIN_H