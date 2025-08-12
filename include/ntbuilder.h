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
 *  - newnet: Initialize the network structure.
 *  - buildnet: Allocate and connect network neurons and buffers.
 * 
 * NOTE: Functions integrate memtrack system from ntmemory, no manual free memory
 * needed, check ntmemory header for more deatils.
 */

#ifndef NTBUILDER_H
#define NTBUILDER_H

#include "ntcore.h"

/**
 * @brief Initialize a network structure with neuron counts per layer.
 *
 * Allocates and copies neuron counts into the `net_t` structure.
 * Internal pointers (`in`, `nn`, `bff`, `out`) are initialized to NULL.
 *
 * @param net Pointer to a pre-allocated `net_t`.
 * @param neurons_per_layer Array with neuron counts per layer.
 *
 * @return Pointer to the initialized `net_t`.
 */
struct ntnet *newnet(net_t *net, unsigned int *neurons_per_layer);

/**
 * @brief Build the neural network by allocating neurons, inputs,
 * buffers, and outputs; and connecting layers accordingly.
 *
 * @param net Pointer to an initialized `net_t`.
 *
 * @return Pointer to the fully built `net_t`.
 */
struct ntnet *buildnet(net_t *net);

#endif // NTBUILDER_H

/*
================================================================================
1. INITIALIZING A NETWORK (newnet)
================================================================================

The `newnet` function prepares the network structure for building by setting up
the number of neurons per layer and initializing internal pointers to NULL.

Example:

    net_t net = { .inputs = 3, .layers = 2 };
    unsigned int neurons[net.layers] = { 4, 2 };

    newnet(&net, neurons);

================================================================================
2. BUILDING THE NETWORK (buildnet)
================================================================================

The `buildnet` function allocates all internal structures: neuron arrays, input
pointers, intermediate buffers, and output pointers. It sets up the connections
between layers, allowing the network to be used immediately.

Example:

    buildnet(&net);

    // Now net.nn, net.in, net.bff, and net.out are allocated and connected.

================================================================================
3. NOTES
================================================================================

- Both functions require the caller to manage the `net_t` structure lifetime.
- Memory allocation is tracked and can be freed via the memory manager (ntmemory.h).
- Designed for feedforward layered networks but can be extended in future.

*/

