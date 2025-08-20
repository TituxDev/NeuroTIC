/**
 * ntbuilder.h - Neural network construction utilities for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * Declares functions to initialize and build feedforward neural networks
 * using the `net_t` structure. Supports flexible layer counts and neuron
 * distributions.
 *
 * Functions:
 *  - newnet   : Initialize the network structure with layer neuron counts.
 *  - buildnet : Allocate and connect neurons, buffers, and outputs.
 *
 * Memory management:
 *  - All allocations are tracked by the `memtrack()` system.
 *  - No manual `free()` calls are required; use `memfree()` if needed.
 */

#ifndef NTBUILDER_H
#define NTBUILDER_H

#include "ntcore.h"

/**
 * @brief Initialize a network structure with neuron counts per layer.
 *
 * Allocates and stores the number of neurons for each layer in `net_t`.
 * Internal pointers (`in`, `nn`, `bff`, `out`) are initialized to NULL,
 * leaving full construction to `buildnet`.
 *
 * @param net Pointer to a pre-allocated `net_t`.
 * @param neurons_per_layer Array containing neuron counts per layer.
 *
 * @return Pointer to the initialized `net_t`.
 */
struct ntnet *newnet (net_t *net , uint16_t *neurons_per_layer );

/**
 * @brief Build the neural network by allocating neurons, inputs, buffers, and outputs.
 *
 * Connects neurons across layers according to `bff_wiring`:
 *  - Allocates `nn` arrays if not already allocated.
 *  - Allocates `in` array for network inputs.
 *  - Allocates intermediate buffers (`bff`) between layers.
 *  - Allocates `out` array for network outputs.
 *  - Sets neuron input pointers to proper buffers or external inputs.
 *
 * @param net Pointer to an initialized `net_t`.
 *
 * @return Pointer to the fully built `net_t`.
 */
struct ntnet *buildnet( net_t *net );

#endif // NTBUILDER_H

/*
================================================================================
1. INITIALIZING A NETWORK (newnet)
================================================================================

Prepares the network structure for building by setting up neuron counts
and initializing internal pointers.

Example:

    net_t net = { .inputs = 3, .layers = 2 };
    unsigned int neurons[net.layers] = { 4, 2 };

    newnet(&net, neurons);

================================================================================
2. BUILDING THE NETWORK (buildnet)
================================================================================

Allocates all internal structures: neuron arrays, input pointers,
intermediate buffers, and output pointers. Sets up connections between layers.

Example:

    buildnet(&net);

    // Now net.nn, net.in, net.bff, and net.out are fully allocated and connected.

================================================================================
3. NOTES
================================================================================

- Caller must manage the `net_t` structure lifetime.
- Memory allocation is tracked by `memtrack()`.
- Designed for feedforward layered networks; can be extended for other topologies.
*/

