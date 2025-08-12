/**
 * ntactivation.h - Activation function definitions for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * This header declares the activation function identifiers, their
 * corresponding function pointers (including derivatives), and
 * initialization ranges used in the NeuroTIC framework.
 * 
 * IMPORTANT: The implementation of some activation functions uses
 *            <math.h>. When compiling/linking your program, you
 *            must link against the math library:
 *
 *                gcc myprog.c ntactivation.c -lm
 *
 *            On some systems (e.g., MSVC), this is not necessary,
 *            but on POSIX systems (Linux, BSD, macOS) it is required.
 */

#ifndef NTACTIVATION_H
#define NTACTIVATION_H

/**
 * @enum ntact_function_id_t
 * @brief Identifiers for available activation functions.
 *
 * Each activation function ID corresponds to both its main function
 * and its derivative, stored in the activation function pointer table.
 */
typedef enum {
    NTACT_BOOLEAN,   ///< Boolean step activation function
    NTACT_SIGMOID,   ///< Sigmoid activation function

    NTACT_TOTAL_FUNCTIONS ///< Total number of activation functions
} ntact_function_id_t;

/**
 * @brief Activation function table.
 *
 * Indexed as:
 *   ntact_activation[function_id][version]
 *
 * - function_id: One of ntact_function_id_t
 * - version: 0 = main activation, 1 = derivative
 *
 * Each entry is a pointer to a function taking a float input and
 * returning a float output.
 */
extern float (*ntact_activation[NTACT_TOTAL_FUNCTIONS][2])(float);

/**
 * @brief Recommended random initialization ranges for each activation function.
 *
 * Indexed as:
 *   ntact_rand_range[function_id][bound]
 *
 * - function_id: One of ntact_function_id_t
 * - bound: 0 = minimum value, 1 = maximum value
 *
 * These ranges can be used when initializing neuron weights for a given
 * activation function to ensure stable learning dynamics.
 */
extern float ntact_rand_range[NTACT_TOTAL_FUNCTIONS][2];

#endif // NTACTIVATION_H

/*
 1. Activation Functions:

     Each activation function transforms the weighted sum of inputs into
     an output value. NeuroTIC stores them in a two-dimensional table:

         ntact_activation[function_id][version]

     - function_id: Index of the function (e.g., NTACT_BOOLEAN, NTACT_SIGMOID).
     - version: 0 = main activation, 1 = derivative.

     This structure allows:
       - Easy switching between activation types.
       - Direct access to derivatives for backpropagation or analysis.

  Example: Using an activation function and its derivative
  --------------------------------------------------------

     float input_value = 0.7f;

     // Get output from sigmoid activation
     float output = ntact_activation[NTACT_SIGMOID][0](input_value);

     // Get derivative of sigmoid at the same input
     float derivative = ntact_activation[NTACT_SIGMOID][1](input_value);


  2. Random Initialization Ranges:

     To improve learning stability, each activation function has a recommended
     range for initializing weights:

         ntact_rand_range[function_id][bound]

     - bound: 0 = minimum value, 1 = maximum value.

     These values can be used when creating new neurons or networks.

  Example: Initializing weights for a sigmoid neuron
  ---------------------------------------------------

     int min_w = ntact_rand_range[NTACT_SIGMOID][0];
     int max_w = ntact_rand_range[NTACT_SIGMOID][1];

     for (int i = 0; i < neuron.inputs; ++i) {
         neuron.w[i] = random_between(min_w, max_w);
     }


  3. Integration with net_t (from ntcore.h):

     When building a network (net_t), each neuron’s `fn` field stores the
     activation function ID. This allows the neuron’s output calculation loop
     to directly fetch the corresponding function from ntact_activation[][].

     Example: Loop for calculating all neurons in a network layer
     -------------------------------------------------------------

     for (int L = 0; L < net.layers; ++L) {
         for (int N = 0; N < net.neurons[L]; ++N) {
             neuron_t *nrn = &net.nn[L][N];

             float sum = 0.0f;
             for (int i = 0; i < nrn->inputs; ++i)
                 sum += *(nrn->in[i]) * nrn->w[i];

             sum += nrn->b;

             nrn->out = ntact_activation[nrn->fn][0](sum);
         }
     }

  This design keeps the activation logic centralized and makes it easy
  to extend the system by adding new functions.
*/

