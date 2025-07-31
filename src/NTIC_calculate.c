/**
 * @note This file is part of NeuroTIC, a neural network library.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file calculate.c
 * @brief Implementation of neural network calculations.
 * @details
 * This file contains the implementation of functions for calculating the output
 * of neurons and running the neural network. It includes the `weighing` function
 * that computes the weighted sum of inputs, the `evaluate` function that applies
 * the activation function to the weighted sum, and the `run_net` function that processes
 * the entire network by evaluating each neuron in each layer.
 * The functions are designed to work with the neuron and net structures defined in
 * `neuroticore.h` and the activation functions declared in `activation.h`.
 * Future revisions may include optimizations for batch processing or parallel execution
 * of neuron evaluations.
 */

#include "calculate.h"

#include "neuroticore.h"
#include "activation.h"

/**
 * @fn weighing
 * @brief Computes the weighted sum of inputs for a neuron.
 * @param N The neuron structure containing inputs and weights.
 * @return The weighted sum of inputs, which is the sum of each input multiplied by its
 *         corresponding weight, plus the neuron's bias.
 * @details
 * This function calculates the weighted sum of inputs for a given neuron.
 * It iterates over the neuron's inputs, multiplying each input value by its corresponding weight,
 * and adds the neuron's bias to the total. The result is the weighted sum that will
 * be passed to the activation function for further processing.
 * The function assumes that the neuron's input pointers (IN) and weights (W) are
 * properly initialized and that the number of inputs matches the number of weights.
 * The function returns the computed weighted sum as a float value.
 */
float weighing( struct neuron N ){
    float w= N.B;
    for( int i= 0; i < N.inputs ; i++ ) w+= *N.IN[i] * N.W[i];
    return w;
}

/**
 * @fn evaluate
 * @brief Evaluates a neuron by applying the activation function to its weighted input.
 * @param N The neuron structure to be evaluated.
 * @return The output of the neuron after applying the activation function.
 * @details
 * This function evaluates a neuron by first calculating the weighted sum of its inputs
 * using the `weighing` function. It then applies the appropriate activation function
 * from the `activation` array based on the neuron's function type and returns the result.
 * The output is stored in the neuron's OUT field, which can be used for further processing
 * or as input to other neurons in the network.
 */
float evaluate( struct neuron *N ){
   return N->OUT= activation[N->FUNC][0]( weighing( *N ) );
}

/**
 * @fn run_net
 * @brief Processes the entire neural network by evaluating each neuron in each layer.
 * @param Net The neural network structure containing layers and neurons.
 * @return A pointer to the output buffer of the network after processing all neurons.
 * @details
 * This function iterates over each layer of the neural network and evaluates each neuron
 * within that layer using the `evaluate` function. It processes all neurons in all layers,
 * ensuring that the outputs are computed based on the current inputs and weights.
 * The final outputs of the network can be accessed through the OUT field of the net structure.
 */
float *run_net( struct net *Net ){
    for( int i= 0, j ; i < Net->layers ; i++ ) for( j= 0 ; j < Net->neurons[i] ; j++ ) evaluate( &Net->N[i][j] );
    return *Net->OUT;
}