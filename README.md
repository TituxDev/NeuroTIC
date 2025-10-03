# NeuroTIC

**NeuroTIC** is a minimalist neural network library written in **pure C** — for those who like their code raw, their terminals dark, and their neurons connected by hand.  
Think of it as soldering logic gates... but with pointers.

---

## 🧠 What is NeuroTIC?

NeuroTIC is a lightweight, modular neural network system built entirely in ANSI C — no external libraries, no hidden magic.  
It was born from the desire to understand how AI works from the ground up, and evolved into a tool for learning, hacking, and creating neural networks manually and dynamically.

If you're curious about the journey behind this system, visit [NeuroTIClab](https://neuroticlab.com) — a companion site (in Spanish) documenting the project’s development.

---

## ✨ Features

- 🧠 Core neuron design: Flexible neuron structs allow you to connect neurons in any topology and assign activation functions dynamically.
- 💾 Full network persistence: Save and load neural networks to and from binary files with a single call — all weights, biases, and wiring are preserved.
- 🔧 All-in-one toolkit: Build, train, and run feedforward networks; helper functions and scripts automate compilation and testing.
- ⚡ On-the-fly editing: Change network structure or parameters dynamically during runtime.
- 🧹 Memory tracking: Built-in memory management ensures clean allocation and deallocation.
- 📜 Script-powered framework: Bash scripts and Makefile handle project setup, compilation, and examples automatically — no hidden dependencies.

---

## 🚀 Quick Start

### Clone the repository and install dependencies

(You can also just download the Makefile and use that)

``` bash
make install
```

This will:

1. Check your environment
2. Clone the NeuroTIC repository
3. Set up required directories

### 📁 Examples

To compile and run an example (e.g., `logic_gates.c`):

```bash
cd examples
bash ./test.sh logic_gates
```

It will automatically compile and run the selected example using the helper build system.

## 📚 Documentation

Explore the internals and history of NeuroTIC through these documents:

- [Project Philosophy](./docs/PHILOSOPHY.md) – Principles behind NeuroTIC.
- [Development Log](./docs/DEVLOGS.md) – The journey from XOR to full networks.
- [Roadmap](./docs/ROADMAP.md) – What’s done and what’s coming.

## 🧾 License

This project uses the [Mozilla Public License 2.0](./LICENSE).
In short:

- 🆓 You can use it freely (even in closed-source software)
- 🪪 Keep modifications to this code open if redistributed
- 🧾 Don’t remove the author’s name
- 🤝 If you're making profit, consider contacting the author — fair is fair

### 🛠️ About the helper scripts

*All `.sh` and `Makefile` scripts in this repository are provided as helper tools.*  
*They're not part of the NeuroTIC core library and are completely free to reuse or adapt — no license required, just give credit if you feel like it.*

## 👤 Author

Crafted with love, logic, and a healthy dose of malloc() by Oscar Sotomayor.
