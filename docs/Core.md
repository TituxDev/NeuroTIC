# NeuroTIC Core Architecture

## Network struct: net_s

net_s is the complete top-level description of a neuronal network in NeuroTIC. It groups inputs, layers, neurons, and the internal buffers that let data flow from external inputs to final outputs. Its role is not to execute the network but to describe it in a fully explicit way.

NeuroTIC’s design avoids hidden mechanisms. Data paths, references, and intermediate values remain visible and traceable. Within this philosophy, net_s acts as the architectural blueprint of a network: it states which components exist and how they are connected so the entire evaluation process can be inspected and understood.

> net_s elelments:
>
> - inputs: number of external inputs.
> - layers: total number of layers.
> - neurons[]: neuron count per layer (neurons[layer]).
> - \*in[]: pointers to input data sources.
> - nn[][]: matrix of neurons organized by layers.
> - wiring[]: connection descriptions used to build internal buffers.
> - bff: shared buffer referencing all input values used by neurons, avoiding duplication.
> - \*out[]: pointers to the output fields of the last-layer neurons.

The main idea is that all network's operation —from the firs intput to the last output— can be tracked runing trought the references containded in net_s. There aren't impilict transfortmations and hidden fluxes; the structure storage every information required to inspect, debug and modify the network on any level even at runtime.

## Neuron struct: neuron_s

neuron_s represents the smallest executable unit inside a NeuroTIC network. Each instance models a single neuron: a function that gathers its inputs, applies weights and bias, evaluates an activation function, and stores its output. The structure is intentionally minimal and fully explicit, exposing every value involved in the computation.

At its core, a neuron does not own numerical data. Instead, it stores references—pointers to external values that may come from network inputs, buffer arrays, or other neurons. This pointer-based design allows sharing inputs between neurons, eliminates data copies, and makes internal flow completely inspectable.

> Fields in neuron_s:
>
> - inputs: number of input connections.
> - bff_idx: index of the buffer array from which this neuron reads its inputs.
> - in: array of pointers to input values; in[i] points to the source of the i-th connection.
> - w: weight array of length inputs.
> - b: bias term applied after the weighted sum.
> - fn: activation function selector (see ntactivation.h).
> - out: output value after activation, written during evaluation.

Although this structure is normally populated automatically inside a net_s network, it can also be instantiated by hand and evaluated directly. The file examples/neurons.c demonstrates this standalone mode by constructing three neurons that implement simple logical gates, wiring them exclusively through direct pointer assignments. The example highlights two design intentions: the structure is self-sufficient, and data flow remains accessible at all times.

## Wiring struct: wiring_s

wiring_s is a declarative specification that tells the builder how to assemble a group of buffer arrays inside the network. These arrays may collect values from external inputs, internal buffers, neuron outputs, or any combination of them. The structure does not enforce layer adjacency or sequential flow; it simply defines how a particular buffer region must be constructed, regardless of where it lies in the topology.

A wiring block does not store numerical values. Instead, it provides all metadata needed to synthesize the corresponding entry in net_s.bff. Once the builder resolves this blueprint, neurons in subsequent regions use these buffers as their input sources.

> Fields in wiring_s:
>
> - arrays: number of buffer arrays to create.
> - array_type: classification of each array and how its elements should be sourced.
> - size: number of elements in each array.
> - src_type, src_layer, src_index: per-element source descriptors for mixed arrays, enabling heterogeneous compositions.

The structure’s emphasis is flexibility. It supports traditional feed-forward layouts, but is equally capable of expressing bypasses, shared buffer regions, or unconventional computational graphs. wiring_s simply defines the “shape” and “origin rules” of a buffer region; the execution logic never touches it. Once resolved into actual pointers by the builder, the network becomes fully operational and the wiring description acts as a persistent, inspectable record of how that region was constructed.

> **See also:**  
> [Data Flow System](Wiring.md) for  complete connetion explanation