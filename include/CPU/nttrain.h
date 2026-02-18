/**
 * @file nttrain.h
 * @brief Header for training utilities in NeuroTIC
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
 * @param traindata_t Pointer to training data structure to initialize.  
 * @param net_s Pointer to the network being trained.
 */
void newtraindata( traindata_t *train_data , net_s *net );

/**
 * @brief Trains a network using backpropagation.
 *
 * @param net Pointer to the network to train.
 * @param train_data Pointer to the training data.
 * @return attempts_t Number of iterations performed.
 */
attempts_t backpropagation( net_s *net , traindata_t *data );

#endif // NTTRAIN_H