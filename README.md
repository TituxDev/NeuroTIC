# NeuroTIC

**NeuroTIC** is a minimalist neural network library written in **pure C** — for those who like their code raw, their terminals dark, and their neurons connected by hand.  
Think of it as soldering logic gates... but with pointers.

---

## 🧠 What is NeuroTIC?

NeuroTIC is a lightweight, modular neural network system built entirely in ANSI C, without dependencies.  
It was born from the desire to understand how AI works from the ground up, and evolved into a tool for learning, hacking, and creating neural networks manually and dynamically.

If you're curious about the journey behind this system, visit [NeuroTIClab](https://neuroticlab.com) — a companion site (in Spanish) documenting the project’s development.

---

## ✨ Features

- 🧠 Single-header neural network system
- 🔌 Manual topology design (your neurons, your wiring)
- 🛠️ Fully dynamic: build any feedforward structure
- 📉 Backpropagation training built-in
- 🧽 DIY memory management (you bring the `free()`)

---

## 📁 Examples

To compile and run an example (e.g., `logic_gates.c`):

```bash
gcc examples/logic_gates.c -lm && ./a.out
```

Adjust training parameters or change logic targets directly in the source file. More examples are available in the examples/ directory.

## 📚 Documentation

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

## 👤 Author

Crafted with love, logic, and a healthy dose of malloc() by Oscar Sotomayor.
