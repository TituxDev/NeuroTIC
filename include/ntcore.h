/**
 * ntcore.h - Core data structures for NeuroTIC neural network system
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * This header defines the fundamental structures for neurons, network wiring,
 * and complete network representations used in the NeuroTIC framework.
 * The design emphasizes simplicity, clarity, and flexibility for manual
 * and automated network construction.
 */

#ifndef NTCORE_H
#define NTCORE_H

#include <stdint.h>

/**
 * @brief Represents a single neuron within a neural network.
 *
 * This structure can be used independently or as part of a network.
 * Inputs are represented as pointers to float values to allow flexible
 * source connections. Weights are stored in an array corresponding to inputs.
 * The output is stored as a single float value.
 *
 * The `fn` field stores the index of the activation function
 * (see `ntactivation` module for available functions).
 *
 * The `bff_idx` field indicates the index of the buffer wiring array
 * to which this neuron is assigned within the network's BFF wiring configuration.
 */
typedef struct ntneuron {
    uint32_t inputs;    // Number of inputs to this neuron.
    uint16_t bff_idx;   // Index in the BFF wiring array associated with this neuron.
    float **in;             // Array of pointers to input values.
    float *w;               // Array of weights for each input.
    float b;                // Bias value.
    uint8_t fn;        // Activation function index.
    float out;              // Stored output value of the neuron.
} neuron_t;

/**
 * @brief Represents a BFF wiring configuration for a layer connection.
 *
 * Each BFF wiring entry describes how arrays of inputs are sourced and connected.
 * Multiple arrays can be defined for complex wiring schemes, each with its type,
 * size, and source mapping.
 *
 * - `array_type` defines the purpose or semantics of each array (e.g., 'M' for main).
 * - `src_type` specifies the type of each source element ('N' for neuron, etc.).
 * - `src_layer` and `src_index` map each element to its origin in the network.
 */
typedef struct ntbffwiring {
    uint16_t arrays;         // Number of arrays in this wiring set.
    uint8_t *array_type;   // Array of array type codes.
    uint32_t *size;          // Array of element counts per array.
    uint8_t **src_type;    // Source type for each array element.
    uint16_t **src_layer;    // Source layer index for each array element.
    uint16_t **src_index;    // Source neuron index for each array element.
} bffwiring_t;

/**
 * @brief Represents a complete neural network.
 *
 * The network consists of multiple layers, each containing a number of neurons.
 * Inputs and outputs are represented as arrays of pointers to allow flexible
 * data flow. Buffers provide intermediate storage, and wiring structures
 * facilitate connections between layers.
 *
 * The `bff_wiring` structure holds abstract connection instructions which can
 * be processed by network builders to set up the `bff` pointer matrices.
 */
typedef struct ntnet {
    uint32_t inputs;     // Number of inputs to the entire network.
    uint16_t layers;     // Number of layers in the network.
    uint16_t *neurons;   // Array of neuron counts per layer.
    float **in;              // Array of pointers to the network inputs.
    neuron_t **nn;           // Matrix (layers x neurons) of neurons.
    bffwiring_t *bff_wiring; // Abstract wiring instructions for each layer connection.
    float ****bff;           // Pointer matrices; each row points to arrays of neuron outputs.
    float **out;             // Array of pointers to network output values.
} net_t;

#endif // NTCORE_H


/*
================================================================================
1. INDIVIDUAL NEURONS
================================================================================

Each neuron (`neuron_t`) manages its own inputs, weights, bias, activation
function, output, and its association with a buffer wiring index (`bff_idx`).

Connections are established by assigning the neuron's `in` pointers to point
to output variables from other neurons or to external input values.

Example (pseudocode):

    float inputA, inputB;
    neuron_t neuron1, neuron2, neuron3;

    // Set up inputs for neuron1 from external sources
    neuron1.inputs = 2;
    neuron1.in = { &inputA, &inputB };
    neuron1.w = { 0.2f, 0.3f };
    neuron1.b = -0.1f;
    neuron1.fn = FN_BOOLEAN;
    neuron1.bff_idx = 0;

    // Set up inputs for neuron2 from external sources
    neuron2.inputs = 2;
    neuron2.in = { &inputA, &inputB };
    neuron2.w = { -0.5f, -0.6f };
    neuron2.b = 0.9f;
    neuron2.fn = FN_BOOLEAN;
    neuron2.bff_idx = 0;

    // neuron3 takes input from neuron1 and neuron2 outputs
    neuron3.inputs = 2;
    neuron3.in = { &neuron1.out, &neuron2.out };
    neuron3.w = { 0.4f, 0.5f };
    neuron3.b = -0.8f;
    neuron3.fn = FN_BOOLEAN;
    neuron3.bff_idx = 0;


================================================================================
2. NETWORK STRUCTURE (net_t)
================================================================================

The network groups neurons in layers and manages inputs, outputs, intermediate
buffers, and high-level wiring instructions.

- `nn` is a matrix of neurons indexed by layer and neuron within layer.
- `bff_wiring` stores connection blueprints for building `bff`.
- `bff` stores actual pointers linking outputs from one layer as inputs to the next.
- `in` points to the external inputs to the network.
- `out` points to the final output neuron outputs.

Connections inside the network are typically established by processing the
`bff_wiring` data to populate `bff` and link neurons accordingly.

-------------------------------------------------------------------------------
AUTOMATED NETWORK CONSTRUCTION (pseudocode)
-------------------------------------------------------------------------------

allocate nn[ layers ]
for each layer L:
    nn[L] = allocate array of neuron_t of size neurons[L]

allocate in = array of float* of size inputs

// Configure first layer neurons (connect to network inputs)
for each neuron N in layer 0:
    N.inputs = network.inputs
    N.in = network.in
    N.w = allocate float array of size network.inputs
    N.bff_idx = assigned wiring index

// Allocate intermediate buffers (link layer outputs to next layer inputs)
allocate bff = array of float** of size (layers - 1)
for each layer L from 0 to layers-2:
    bff[L] = allocate array of float* of size neurons[L]
    for each neuron N in layer L:
        bff[L][N] = &nn[L][N].out

// Configure hidden and output layers (connect to previous layer's outputs)
for each layer L from 1 to layers-1:
    for each neuron N in layer L:
        N.inputs = neurons[L - 1]
        N.in = bff[L - 1]
        N.w = allocate float array of size neurons[L - 1]
        N.bff_idx = assigned wiring index

// Allocate network outputs (point to last layer's neuron outputs)
allocate out = array of float* of size neurons[layers - 1]
for each neuron N in last layer:
    out[N] = &nn[last_layer][N].out


================================================================================
3. DIAGRAM OF DATA FLOW
================================================================================

                [ External Inputs ]
                in[0]   in[1]   in[2]
                  |       |       |
                  v       v       v
         +-------------------------------+
Layer 0: | nn[0][0]   nn[0][1]   nn[0][2] |
         +-------------------------------+
            | out        | out      | out
            v            v          v
         bff[0][0]   bff[0][1]  bff[0][2]
            \           |           /
             v          v          v
         +-------------------------------+
Layer 1: | nn[1][0]   nn[1][1]   nn[1][2] |
         +-------------------------------+
            | out        | out      | out
            v            v          v
         bff[1][0]   bff[1][1]  bff[1][2]
            \           |           /
             v          v          v
         +-------------------------------+
Layer 2: | nn[2][0]   nn[2][1]   nn[2][2] |
         +-------------------------------+
            | out        | out      | out
            v            v          v
          out[0]      out[1]     out[2]

Legend:
-------
in[]        -> Pointers to network inputs (external variables)
nn[][]      -> Matrix of neurons indexed by layer and neuron number
bff[][]     -> Buffers storing pointers to outputs of previous layer neurons
bff_wiring  -> Abstract wiring configuration for each layer connection
out[]       -> Pointers to network outputs (final layer neuron outputs)


================================================================================
4. ADVANTAGES OF net_t
================================================================================

- **Compact iteration**: Layers and neurons can be traversed with nested loops:
      for (L = 0; L < net.layers; ++L)
          for (N = 0; N < net.neurons[L]; ++N)
              process(&net.nn[L][N]);

- **Automatic wiring**: No need to manually connect each neuron when using
  automated allocation logic based on `bff_wiring`.

- **Flexible integration**: External systems can directly read/write from
  `in[]` and `out[]` without altering internal structures.

- **Readable architecture**: The `nn` matrix mirrors the conceptual layout
  of a layered network, making debugging and visualization easier.
*/

