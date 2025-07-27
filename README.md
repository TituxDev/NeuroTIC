# NeuroTIC

**NeuroTIC** is a minimalist neural network library written in **pure C** — for those who like their code raw, their terminals dark, and their neurons connected by hand.  
Think of it as soldering logic gates... but with pointers.

## Overview

This project started with a spark of curiosity — the urge to understand how AI truly works, from the ground up. The challenge? Build everything in plain C, without external libraries or frameworks.

It all began with XOR. Once that classic hurdle was cleared, the need for custom tools to create and connect neurons became inevitable. From there, NeuroTIC evolved into a modular, hackable neural network system — lightweight, low-level, and completely under your control.

It’s not meant to compete with TensorFlow... **yet**.
But give it enough neurons, enough caffeine, and just the right number of segfaults, and who knows?

If you're curious about how this code evolved, check out [NeuroTIClab](https://neuroticlab.com)— a site (in Spanish) that documents the process from scratch.

## 🧭 Philosophy

NeuroTIC is not just a tool — it's an experiment in clarity, control, and curiosity. It's built with the following principles:

- 🔍 Understand before you abstract.
- 🛠️ Build it yourself — you’ll know how it breaks.
- 📦 No dependencies, no magic — just C, malloc, and logic.
- 🧑‍💻 Software should be free, transparent, and respectful of its creators.
- 🎓 Made for learning, hacking, and growing neural networks from scratch.

---

## Features

- 🧠 Single-header neural network system
- 🔌 Manual topology design (your neurons, your wiring)
- 🛠️ Fully dynamic: build any feedforward structure
- 📉 Backpropagation training built-in
- 🧽 DIY memory management (you bring the free())

## Examples

Examples are located in the examples/ directory.

To compile and run an example (e.g., logic_gates.c):

``` bash
gcc examples/logic_gates.c -lm && ./a.out
```

Check the logic_gates.c file to adjust training parameters or change the target logic function.

## Roadmap

- ✅ Safe and responsible memory management.
- 🔌 Connection registry for exotic topologies.
- 💾 Save/load format for trained networks.
- 🧯 Internal safety checks and input validation.
- 📁 Restructure and organize the library using source code files.

---

## Dev Log

A brief and possibly dramatic history of the madness behind the code.

- 🧠 AI can't be an infinite number of nested ifs. How does it really work?
- 🛠️ Boolean perceptron looks easy to be coded.
- ⚖️ Everything is just weights, biases, and error calculations... to automate the adjustments.
- ❌ Why can’t XOR be solved!?
- 🧮 Oh, I need more neurons.
- 🏋️ How do you train a full network?
- 🛠️ I need tools to build and train networks.
- 🚀 This is starting to feel like a real system! Let's go deeper.
- 🧱 First, neurons must be treated as objects — let’s encapsulate their properties in a structure.
- 🔌 What if their connections emulate electrical wiring, using inputs as pointers and outputs as variables?
- 🗂️ If I want to create flexible functions to control the network... I need structure. Time to build a network struct with a dynamic neuron matrix, topology info, and external connections.
- 🕸️ With pointers and dynamic memory — any topology is possible!
- 🧼 Right... but I have to respect memory space. Time to add buffers between layers.

---

## License

This project uses the [Mozilla Public License 2.0](https://www.mozilla.org/en-US/MPL/2.0/). In short:

- 🆓 You can use it freely (even in closed-source software)
- 🪪 Keep modifications to this code open if redistributed
- 🧾 Don’t remove the author’s name (respect the nerd behind it)
- 🤝 Consider contacting the author if you’re making profit — fair is fair

## Credits

Crafted with love, logic, and a healthy dose of malloc() and pointers by Oscar Sotomayor.
