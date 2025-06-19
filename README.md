# Concept Forge - OpenGL Game Engine

https://github.com/user-attachments/assets/f1aa9311-87a4-464d-94db-ae95a1214562


*Above: A demo of early simulation logic (The Bees and Bombs algo) written by Claude AI running in python binded Concept Forge*

---

**Concept Forge** is a lightweight, modular game/simulation engine built using modern **C++**, **OpenGL**, and **Python**.
Designed for performance, flexibility, and expressiveness, it allows you to prototype and
build everything from real-time simulations to experimental gameplay systems — with full control from both C++ and Python, and the
main thing, **Natural Langauge!!**

Just say the word, and it'll do it for you.

Built in public. Built to scale.  
An evolving foundation for creative and technical experiments.

---

## Features

- **AI Focused**: Built to include AI Generation from the ground up.
- **Custom Engine Core**: Built from scratch with modern C++ and OpenGL.
- **Python Scripting Layer**: Bindings via [`nanobind`](https://github.com/wjakob/nanobind) let you control the engine from Python.
- **Hybrid Architecture**: Use C++ for speed, Python for iteration.
- **Cross-platform**: Works on Windows, Linux, and macOS with minimal dependencies.
- **Extensible**: Modular systems allow you to build tools, logic, and rendering systems on top.

---

## Installation

### Common Steps

```bash
git clone --recurse-submodules https://github.com/kshitijaucharmal/OpenGL-GE
cd OpenGL-GE
mkdir build
cd build
````

---

### Linux / macOS
```bash
cmake -DPython_EXECUTABLE=$(which python3.10) ..
make
make run  # to execute
```

### Windows
```bash
cmake -G "MinGW Makefiles" ..
mingw32-make
mingw32-make run  # to execute
```

> Ensure Python 3.10 is installed and available in your PATH, or at least `which python3.10` points to it.

---

## Contributing

Pull requests and issues are welcome.
Whether you're optimizing low-level performance or exploring experimental ideas, you're invited to build on this.

---

## License

This project is licensed under the **MIT License**.
See [`LICENSE`](./LICENSE) for full details.

---

## 🔗 Links

* [Twitter (Build Logs)](https://twitter.com/ma_at_anubis)
* [Nanobind](https://github.com/wjakob/nanobind)
* [GLFW](https://www.glfw.org/) / [GLAD](https://glad.dav1d.de/)

---

> Created by Kshitij Aucharmal([@ma\_at\_anubis](https://x.com/ma_at_anubis))
