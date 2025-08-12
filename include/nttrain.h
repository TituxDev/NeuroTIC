/**
 * nttrain.h - Training data structures and backpropagation training for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * This header declares the training data structure and functions to allocate training data
 * buffers and perform network training using backpropagation.
 *
 * Memory management is handled internally via ntmemory; manual freeing is not required.
 */

#ifndef NTTRAIN_H
#define NTTRAIN_H

#include "ntcore.h"

/**
 * @struct traindata_t
 * @brief Holds training samples, expected results, and training parameters.
 *
 * Fields:
 * - `samples`: Number of training samples.
 * - `learning_rate`: Step size for weight updates.
 * - `tolerance`: Target error threshold for early stopping.
 * - `max_attempts`: Maximum number of training iterations.
 * - `in`: Array of pointers to input vectors.
 * - `results`: Array of pointers to expected output vectors.
 */
typedef struct nttraindata {
    unsigned int samples;
    float learning_rate;
    float tolerance;
    unsigned int max_attempts;
    float **in;
    float **results;
} traindata_t;

/**
 * @brief Allocate memory for training data arrays.
 *
 * Initializes the `in` and `results` buffers based on the network's input size
 * and the number of neurons in the output layer.
 *
 * Memory is tracked and freed automatically; manual deallocation is unnecessary.
 *
 * @param train_data Pointer to training data structure to initialize.
 * @param net Pointer to network whose input and output sizes define buffer sizes.
 *
 * Example usage:
 * @code
 * traindata_t data = { .samples = 100, .learning_rate = 0.01f, .tolerance = 0.001f, .max_attempts = 1000 };
 * newtraindata(&data, &net);
 * // Fill data.in and data.results here...
 * @endcode
 */
void newtraindata( traindata_t *train_data , net_t *net );

/**
 * @brief Train the network using backpropagation.
 *
 * Runs training cycles over the dataset until error tolerance or max attempts
 * is reached. Updates neuron weights and biases accordingly.
 *
 * @param net Pointer to the neural network to train.
 * @param data Pointer to training data with inputs, expected outputs, and parameters.
 * @return Number of training attempts performed.
 *
 * Example usage:
 * @code
 * int attempts = backpropagation(&net, &data);
 * printf("Training completed in %d attempts\n", attempts);
 * @endcode
 */
int backpropagation( net_t *net , traindata_t *data );

#endif // NTTRAIN_H

/*
================================================================================
1. STRUCTURE traindata_t
================================================================================

This structure holds training inputs, expected results, and hyperparameters:

- `samples`: number of training examples.
- `in`: pointer to array of input vectors (each of size network inputs).
- `results`: pointer to array of expected outputs (each matches output layer size).
- `learning_rate`: controls weight update magnitude.
- `tolerance`: error threshold to stop training early.
- `max_attempts`: maximum iterations to try.

================================================================================
2. ALLOCATING TRAINING DATA
================================================================================

`newtraindata()` sets up memory buffers for training samples and expected outputs.
It uses `ntmemory` for automatic tracking.

Example:

    traindata_t data = { .samples = 50, .learning_rate = 0.05f, .tolerance = 0.01f, .max_attempts = 500 };
    newtraindata(&data, &net);

Fill `data.in[i]` and `data.results[i]` with training data before training.

================================================================================
3. TRAINING WITH BACKPROPAGATION
================================================================================

`backpropagation()` performs the training process on the given dataset,
adjusting weights to minimize error.

Returns the number of iterations performed until convergence or max attempts.

Example:

    int attempts = backpropagation(&net, &data);
    if (attempts < data.max_attempts)
        printf("Training converged in %d iterations.\n", attempts);
    else
        printf("Training stopped after reaching max attempts.\n");

*/
