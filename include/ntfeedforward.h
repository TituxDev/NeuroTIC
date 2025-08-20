/**
 * ntfeedforward.h - Feedforward network wiring declarations for NeuroTIC
 *
 * Author: Oscar Sotomayor
 * License: Mozilla Public License Version 2.0 (MPL-2.0)
 *
 * Declares functions to generate standard feedforward wiring instructions
 * (`bff_wiring`) for neural networks. These instructions are used by `buildnet`
 * to allocate buffers and connect neurons layer by layer.
 *
 * The module currently supports:
 *  - feedforward(): Sets up classic layer-to-layer feedforward connections.
 *
 * Future expansions may include:
 *  - Skip connections
 *  - Dense connections
 *  - Other feedforward variants
 */

#ifndef NTFEEDFORWARD_H
#define NTFEEDFORWARD_H

#include "ntcore.h"

/**
 * @brief Generate feedforward wiring for a network.
 *
 * Configures the `bff_wiring` structure for a standard feedforward network:
 *  - Allocates one buffer array per layer connecting outputs to the next layer's inputs.
 *  - Sets buffer types, sizes, and source neuron information.
 *  - Updates each neuron's `bff_idx` to indicate the buffer it uses.
 *
 * This wiring allows `buildnet` to automatically allocate buffers and connect neurons
 * for a classic feedforward architecture.
 *
 * @param net Pointer to an initialized `net_t` structure.
 *
 * @return Returns the pointer to the same `net_t` with `bff_wiring` configured.
 *
 * @note All memory allocations are tracked via `memtrack()`; no manual free() is needed.
 */
struct ntnet *newfeedforward( net_t *net );

#endif // NTFEEDFORWARD_H

/*
================================================================================
1. FEEDFORWARD WIRING
================================================================================

The `feedforward` function prepares layer-to-layer wiring for a neural network:

    - Each neuron in layer L points to the corresponding neuron output in layer L-1.
    - A single buffer array ('M') is used per layer to store output pointers.
    - `bff_idx` in each neuron indicates the buffer to use.

Example usage:

    net_t net;
    net.inputs = 3;
    net.layers = 3;
    unsigned int neurons[3] = { 4, 5, 2 };
    newnet(&net, neurons);

    feedforward(&net);  // Configure wiring
    buildnet(&net);     // Allocate neurons, buffers, and connections

After execution, `net.bff_wiring` contains instructions for a standard
feedforward network.
*/
