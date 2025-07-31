/**
 * @note This file is part of the NeuroTIC project.
 * @author Oscar Sotomayor
 * @date 2025
 * @version 1.0
 * @license Mozilla Public License v. 2.0 (https://mozilla.org/MPL/2.0/)
 * @file neuroticore.h
 * @brief Core data structures for the NeuroTIC neural network framework.
 * @details
 * This file defines the fundamental data structures `neuron` and `net` used to
 * represent artificial neurons and neural networks within NeuroTIC.
 * The `neuron` struct groups attributes such as inputs, weights, bias, activation
 * function selector, and output. The `net` struct organizes neurons into a layered
 * topology, managing inputs, outputs, buffers, and network layout.
 * This header only contains data structure definitions and no function declarations
 * or implementations.
 * Future revisions may include macros or utility functions for network iteration or
 * manipulation.
 */


#ifndef NEUROTICORE_H
#define NEUROTICORE_H

/**
 * @struct neuron
 * @brief Data structure for representing the configuration and state of an artificial neuron.
 *
 * This structure serves as a flexible container for storing the key parameters of an artificial neuron,
 * including its input references, weight values, bias term, activation function selector, and output placeholder.
 *
 * It does not implement any internal logic or perform automatic computations. Instead, it is designed to
 * be controlled entirely by external code that handles memory management, signal processing, and activation.
 *
 * The goal is to provide a modular and reusable definition of a neuron that can be used independently or
 * as part of a neural network, with the freedom to select activation functions, input sources, and initial parameters.
 *
 * @note This structure is passive; its behavior depends entirely on how it is used by external functions.
 *       All computation (such as weighted sums and activation) must be performed explicitly by the user.
 *       For details on usage and precautions, see the field-level documentation either below
 *       (if viewing this file directly) or in the source header file @c neuroticore.h.
 *
 * Field overview:
 * - @c inputs : (unsigned int) Number of expected inputs (informational only).
 * - @c IN     : (float **)     Array of pointers to input values (external sources).
 * - @c W      : (float *)      Array of weights corresponding to each input.
 * - @c B      : (float)        Bias value added to the weighted sum.
 * - @c FUNC   : (unsigned int) Index used to select the activation function (see activation.h).
 * - @c OUT    : (float)        Output value from the most recent computation (must be assigned manually).
 */
struct neuron {
/* === FIELD: unsigned int neuron.inputs ===
 * Purpose:
 *     Indicates the declared number of inputs and weights associated with the neuron.
 *     This value is purely informational and does not enforce any internal constraints
 *     or automatic synchronization with the IN or W fields.
 *
 * Use cases:
 *     + Facilitates external operations such as memory allocation,
 *       iteration over inputs, or validation of neuron configuration.
 *
 * Notes:
 *     + The neuron structure does not internally validate that 'inputs' matches the
 *       actual sizes of IN or W.
 *     + The field is intended as a convenience for manual or generic neuron handling.
 *
 * Recommendations:
 *     + Keep this value manually synchronized with the actual size of IN and W arrays.
 *     + Desynchronization will not break behavior by default, but it may lead to
 *       incorrect assumptions in tools or functions that rely on this field.
 */

    unsigned int inputs;
/* === FIELD: float** neuron.IN ===
 * Purpose:
 *     Array of pointers to external float values used as inputs for the neuron.
 *     Each pointer in the array should reference a valid memory location containing
 *     a single float input value.
 *
 *     This design allows the neuron to be linked dynamically to arbitrary sources of data,
 *     such as buffers, global variables, sensor readings, or outputs from other neurons.
 *     No data is copied — all access is performed by reference.
 *
 * Warnings:
 *     + The array must be allocated and initialized before use.
 *     + If allocating via realloc, ensure it was previously initialized (e.g., to NULL)
 *       to avoid undefined behavior.
 *     + The pointers stored in IN must remain valid throughout the neuron's operation.
 *       Avoid referencing memory that may be freed or modified unexpectedly.
 *
 * Recommendations:
 *     + Common initialization patterns:
 *         - Allocation:
 *             neuron.IN = malloc(neuron.inputs * sizeof(float*));
 *             neuron.IN = calloc(neuron.inputs, sizeof(float*));
 *         - Reallocation (after initialization):
 *             neuron.IN = realloc(neuron.IN, neuron.inputs * sizeof(float*));
 *         - Assignment to an existing array of float pointers:
 *             neuron.IN = source;
 *         - Assigning pointers to individual values:
 *             for (i = 0; i < neuron.inputs; i++)
 *                 neuron.IN[i] = &source[i];
 *           (Make sure 'source' is a valid array of float variables.)
 */

    float **IN;
/* === FIELD: float* neuron.W ===
 * Purpose:
 *     Array of weight values applied to each input during signal processing.
 *     Each weight is multiplied by the corresponding input value:
 *         weighted_input = (*neuron.IN[i]) * neuron.W[i]
 *
 * Warnings:
 *     + The number of elements in neuron.W must match the number of input pointers in neuron.IN.
 *     + Accessing weights beyond the allocated size will result in undefined behavior.
 *
 * Recommendations:
 *     + Allocate memory using:
 *         neuron.W = malloc(neuron.inputs * sizeof(float));
 *         neuron.W = calloc(neuron.inputs, sizeof(float));
 *     + Initialize weights appropriately (e.g., with small random values) before training or inference.
 */
    float *W;
/* === FIELD: float neuron.B ===
 * Purpose:
 *     Bias term added to the neuron's weighted input sum prior to activation:
 *         output_sum = Σ( *neuron.IN[i] * neuron.W[i] ) + neuron.B
 *
 *     The bias acts as an additional degree of freedom that allows the neuron
 *     to shift the activation threshold, improving the model’s ability to learn
 *     complex patterns beyond purely linear combinations.
 *
 * Notes:
 *     + The bias is typically initialized with a small constant or random value.
 *     + It can be updated during training using standard optimization algorithms
 *       such as gradient descent.
 *     + Unlike the weight array, B is a single scalar value.
 */
    float B;
/* === FIELD: unsigned int neuron.FUNC ===
 * Purpose:
 *     Index to select the neuron's activation function from a predefined
 *     array of function pointers.
 *
 *     This field serves as a convenient reference used by external processing
 *     code to determine which activation function to apply.
 *     By itself, it does not modify the neuron's behavior unless the activation
 *     function pointer array is consulted during evaluation.
 *
 * Recommendations:
 *     + Refer to 'activation.h' for predefined function indices and examples.
 *     + If custom activation functions are used or none are applied,
 *       this field can be ignored or repurposed as needed.
 */
    unsigned int FUNC;
/* === FIELD: float neuron.OUT ===
 * Purpose:
 *     Stores the neuron's last calculated output value after applying the activation function.
 *
 *     This value is **not updated automatically** by the neuron structure;
 *     it must be explicitly assigned by the external evaluation code once
 *     the output has been computed.
 *
 * Notes:
 *     + Provides quick access to the neuron's current output for downstream processing.
 *     + Should be updated consistently to reflect the neuron's state after evaluation.
 *     + Other neurons or network layers may reference this value directly via pointers.
 */
    float OUT;
};

/**
 * @struct net
 * @brief Represents a layered artificial neural network as a structured collection of neurons.
 *
 * This structure organizes neurons into layers, forming a matrix-like topology where each row
 * corresponds to a layer and each column to a neuron within that layer.
 * It provides essential metadata about the network's topology — including the number of inputs,
 * number of layers, and neurons per layer — as well as buffers for input data, inter-layer connections,
 * and final outputs.
 *
 * While the structure itself does not implement computation or learning logic,
 * its design enables a semi-automatic flow of data:
 * - External input data can be connected directly to the first layer via the input buffer.
 * - Intermediate buffers between layers minimize data duplication by referencing neuron outputs.
 * - The output buffer aggregates final neuron outputs for easy access by external processes.
 *
 * This facilitates external iteration and processing routines over the network, allowing
 * flexible and efficient evaluation, training, and inference.
 *
 * Fields overview:
 * - @c inputs  : (unsigned int)     Number of input signals accepted by the network.
 * - @c IN      : (float **)         Buffer of pointers serving as the network’s external input interface.
 * - @c layers  : (unsigned int)     Number of neuron layers.
 * - @c neurons : (unsigned int *)   Array specifying the neuron count per layer.
 * - @c N       : (struct neuron **) Matrix of neurons indexed by layer and position.
 * - @c B       : (float ***)        Buffers of pointers connecting outputs of one layer to inputs of the next.
 * - @c OUT     : (float **)         Buffer pointing to the outputs of the final layer neurons.
 */
struct net {
/* === FIELD: unsigned int net.inputs ===
 * Purpose:
 *     Indicates the total number of input signals expected by the network.
 *     This value does not influence the internal behavior of the structure,
 *     but serves as a reference for memory allocation, buffer sizing,
 *     and iteration over input data.
 *
 * Usage Notes:
 *     + Commonly used to allocate or validate the input buffer (IN).
 *     + Serves as a loop limit when assigning or copying values into the IN buffer.
 *     + Useful when building or reconfiguring the network dynamically.
 *
 * Recommendations:
 *     + Ensure consistency between this value and the actual size of the IN buffer.
 *     + Desynchronization may not break functionality in manually managed setups,
 *       but can lead to unsafe behavior when using generic or automated routines.
 */
    unsigned int inputs;
/* === FIELD: unsigned int net.layers ===
 * Purpose:
 *     Indicates the total number of neuron layers contained in the network.
 *     This value does not directly influence the structure's behavior,
 *     but is essential for coordinating the topology and memory layout
 *     of related fields such as neurons, N, B, and OUT.
 *
 * Usage Notes:
 *     + Defines the number of rows in the neuron matrix (N).
 *     + Specifies the size of the neurons array (layer-wise neuron counts).
 *     + Determines the number of pointer buffers in B (which must be of size layers - 1).
 *     + Commonly used as a loop limit when iterating over layers in allocation or processing routines.
 *
 * Recommendations:
 *     + Ensure consistency between this value and the actual size of the neurons and N arrays.
 *     + Desynchronization may not break manual control,
 *       but can lead to misaligned memory, invalid indexing, or logic errors in automated tools.
 */
    unsigned int layers;
/* === FIELD: unsigned int* net.neurons ===
 * Purpose:
 *     Array specifying the number of neurons in each layer of the network.
 *     Each element represents the column count for one row (layer) in the neuron matrix (N).
 *
 * Usage Notes:
 *     + Defines how many neuron structures should be allocated in each layer of N.
 *     + Used to allocate and index pointer buffers in B, and to configure OUT for final output extraction.
 *     + Serves as the upper bound when iterating over neurons within a specific layer.
 *
 * Recommendations:
 *     + The array length should match the total number of layers (`net.layers`).
 *     + Ensure synchronization between this array and the actual structure of N, B, and OUT.
 *       Inconsistencies can cause memory corruption or invalid indexing.
 */
    unsigned int *neurons;
/* === FIELD: float** net.IN ===
 * Purpose:
 *     Buffer of pointers to input data values received from external sources.
 *     This array serves as the entry point for feeding input signals into the network.
 *
 * Usage Notes:
 *     + Neuron inputs (neuron.IN) that require external data should point to elements within this buffer.
 *     + Acts as a shared interface between external data providers and the internal neurons.
 *     + Enables flexible connection and reuse of input data without copying.
 *
 * Recommendations:
 *     + Allocate this buffer with size equal to net.inputs.
 *     + Ensure that the data pointed to by each element remains valid during network evaluation.
 */
    float **IN;
/* === FIELD: struct neuron** net.N ===
 * Purpose:
 *     Two-dimensional array (matrix) of neurons organized by layers and neurons per layer.
 *     Represents the core of the network where each element is a neuron instance.
 *     Facilitates direct access to specific neurons using layer and neuron indices.
 *
 * Usage Notes:
 *     + The number of rows corresponds to net.layers.
 *     + Each row contains a number of neurons specified by net.neurons for that layer.
 *     + Provides structured access to all neurons for processing, training, and evaluation.
 *
 * Recommendations:
 *     + Allocate memory for net.N as an array of pointers to neuron arrays, one per layer.
 *     + Ensure synchronization with net.layers and net.neurons to prevent out-of-bounds access.
 */
    struct neuron **N;
/* === FIELD: float*** net.B ===
 * Purpose:
 *     Array of buffers, each buffer being an array of pointers to neuron outputs (neuron.OUT).
 *     Used to internally connect layers by referencing outputs from one layer as inputs for the next,
 *     avoiding duplication of input arrays in neuron.IN fields and thus facilitating efficient memory management.
 *
 * Usage Notes:
 *     + The size of net.B is typically (net.layers - 1), one buffer per layer connection.
 *     + Each buffer contains pointers equal to the number of neurons in the corresponding source layer.
 *     + Each pointer in a buffer references the OUT field of a neuron in that layer.
 *     + While primarily designed for connecting consecutive layers, it can be adapted for other topologies.
 *
 * Recommendations:
 *     + Ensure buffers are properly allocated with respect to net.neurons and net.layers.
 *     + Maintain synchronization to prevent dangling or invalid pointers.
 */
    float ***B;
/* === FIELD: float** net.OUT ===
 * Purpose:
 *     Buffer of pointers to the output values (neuron.OUT) of the final layer neurons.
 *     Provides a convenient way for external code to access the network's final outputs
 *     after evaluation or inference.
 *
 * Usage Notes:
 *     + The size of net.OUT should equal the number of neurons in the last layer
 *       (net.neurons[net.layers - 1]).
 *     + Each element in net.OUT points directly to the OUT field of a neuron in the last layer.
 *     + This buffer is typically read-only from an external perspective and reflects
 *       the current computed outputs of the network.
 *
 * Recommendations:
 *     + Allocate net.OUT to match the size of the last layer.
 *     + Ensure the outputs of the last layer neurons are updated before reading from this buffer.
 */
    float **OUT;
};

#endif // NEUROTICORE_H
