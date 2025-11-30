# Activation functions

This module defines the activation-function system used by NeuroTIC. Every neuron specifies which activation function it uses by storing an integer label in neuron.fn. That label corresponds to an index inside a global table of function pointers, defined at compile time.

The framework keeps things lean: the persistence format only stores the fn value, never the function pointers themselves. On load, the compiled table is already present and the stored integer simply maps back to the correct function.

## Function table structure

Activation functions are organized in a static array of size NTACT_TOTAL_FUNCTIONS. This size comes directly from the final label inside the enum that lists each activation function identifier. That enum exists so the user has readable names instead of remembering numerical indices.

The table lives in the library’s source file. A matching extern declaration in the header exposes the table so the rest of the framework can call functions through it.

Each entry in the table contains two function pointers:

ntact_activation\[fn][0] – the forward activation function.

ntact_activation\[fn][1] – the derivative of that function.

Both take a single float x parameter, where x is the weighted sum received by the neuron.

## How derivatives work

Some activation functions need the output of the forward function to compute their derivative. To avoid the overhead of storing every neuron’s last output, the derivative call recomputes whatever it needs based only on the raw input x. This keeps the system stateless and easy to integrate with any buffer configuration.

Customizing activation functions

Users can add their own activation functions by extending the enum and enlarging the compile-time table. As long as two functions are provided (forward and derivative) and the new label increments the enum before NTACT_TOTAL_FUNCTIONS, everything plugs into the runtime structure automatically.

## Vectorial activation functions

The system described above only supports scalar activation functions. Vectorial functions, such as softmax, are not managed by this mechanism yet and remain outside the current framework.

It is technically possible to connect buffers to such functions, but this is strongly discouraged unless the developer knows exactly how those vector operations interact with the network’s update flow. Their behavior does not match the assumptions of the scalar activation pipeline.

> **See Also:**  
> ntactivation.h
