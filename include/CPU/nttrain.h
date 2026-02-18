/**
 * @defgroup NTTrain Training Utilities
 * @file nttrain.h
 * @brief Header for training utilities in NeuroTIC
 *
 * Provides structures and functions to store training data and perform
 * supervised learning on feedforward networks.
 * 
 * @author Oscar Sotomayor
 * @date 2024
 */

#ifndef NTTRAIN_H
#define NTTRAIN_H

#include "ntcore.h"

typedef data_t precision_t;
typedef uint64_t sample_t;


/**
 * @brief Structure to hold training dataset and parameters.
 */
typedef struct nttraindata {
    sample_t samples;
    precision_t learning_rate;
    precision_t tolerance;
    uint64_t max_attempts;
    data_t **in;
    data_t **results;
} traindata_t;

/**
 * @brief Allocates memory for training data arrays.
 *
 * Initializes `in` and `results` arrays based on the network structure
 * and number of samples.
 *
 * @param train_data Pointer to training data structure to initialize.
 * @param net Pointer to the network being trained.
 */
void newtraindata( traindata_t *train_data , net_s *net );

/**
 * @brief Trains a network using backpropagation.
 *
 * Updates network weights and biases iteratively based on the training data
 * until the error is below `tolerance` or `max_attempts` is reached.
 *
 * @param net Pointer to the network to train.
 * @param train_data Pointer to the training data.
 * @return Number of iterations performed.
 */
int backpropagation( net_s *net , traindata_t *data );

#endif // NTTRAIN_H