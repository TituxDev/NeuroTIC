/**
 * @file nttrain.h
 * @copybrief nttrain.c
 * 
 * @ref https://tituxdev.github.io/NeuroTIC/src/CPU/nttrain.c
 *
 * @copydetails nttrain.c
 */

#ifndef NTTRAIN_H
#define NTTRAIN_H

#include "ntcore.h"

typedef data_t precision_t;
typedef uint64_t sample_t, attempts_t;


/**
 * @brief Structure to hold training dataset and parameters.
 */
typedef struct traindata_t {
    sample_t samples;           /**< Number of training samples. */
    precision_t learning_rate;  /**< Learning rate for weight updates. */
    precision_t tolerance;      /**< Error tolerance for training convergence. */
    attempts_t max_attempts;    /**< Maximum number of training iterations. */
    data_t **in;                /**< Input data for training samples. */
    data_t **results;           /**< Expected output results for training samples. */
} traindata_t;

/**
 * @brief Allocates memory for training data arrays.
 *
 * @param train_data Pointer to a traindata_t instance with `samples`
 *                    already set.
 * @param net Pointer to the network being trained.
 */
void newtraindata( traindata_t *train_data , net_s *net );

/**
 * @brief Trains a network using backpropagation.
 *
 * @param net Pointer to the network to train.
 * @param train_data Pointer to the training data.
 * @return Number of epochs (full passes over the training set) performed.
 */
attempts_t backpropagation( net_s *net , traindata_t *train_data );

#endif // NTTRAIN_H