
 # NeuroTIC

[GitHub](https://github.com/TituxDev/NeuroTIC)  
[USER GUIDE](https://tituxdev.github.io/NeuroTIC/docs/doxy/result/html/index.html)  
A framework for understanding, exploring, and experimenting with neural networks. `Built in C`, it gives you full visibility into the source code and the freedom to modify, extend, and hack every component.

**NeuroTIC** helps you design, build, train, save, and load networks with complete creative control over their topology—without dragging you through the usual low-level chores.

This workspace is meant for people who care about transparency and control: developers, students, and tinkerers who want to understand neural networks from the inside out and shape them on their own terms.

## About the framework

**NeuroTIC** offers a modular environment where every part of a neural network—layers, neurons, activation flows, learning rules, serialization—is open and accessible.

The design is intentionally minimal: you get a clean API for common tasks, but nothing is hidden.

## Architecture overview

The core idea behind NeuroTIC is simple:
a neural network should be fully visible, fully editable, and never trapped inside a rigid template.

The framework arranges neurons, layers, and data flows in a compact structure where everything is explicit. Nothing happens behind your back, and anything you dislike can be rewritten on the spot.

At the center sits the `net_s` structure. It exposes the external input ports, records the layer count and the neuron count per layer, stores the entire neuron matrix, and carries the wiring descriptors and buffer system that shuttle values between layers. After the build step the network has all connections resolved, the output ports assigned, and the internal memory under its control. External inputs—or any custom data sources you plug in—remain entirely yours. In practice, net_s becomes the fully inspectable device you run, train, save, reload, and tinker with.

Helper modules step in for convenience—dense wiring, feed-forward layouts, activation dispatch, weight initialization, training routines, persistence—but they’re always optional. They accelerate your work without limiting what you can build.

Because the network stays transparent at every stage, you can:

- assemble a classic dense model with a single call,
- mix automatic wiring with handcrafted connections,
- shape exotic topologies layer by layer,
- poke weights and activation paths directly,
- experiment with training rules outside the usual canon,
- and run inference without a single runtime allocation.

NeuroTIC avoids the black-box mentality altogether. Everything arrives as plain C code sitting inside your project, ready for inspection, extension, mutation, or the occasional glorious detonation in the name of learning.

