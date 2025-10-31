
# 🌌 N-Body Simulation (C++ & Raylib)

This project is a **3D N-body gravitational simulation** written in **C++**, using the **Runge-Kutta (RK4)** integration method and rendered in real time with **Raylib**.  
It visually demonstrates how celestial bodies (like planets and stars) interact under Newtonian gravity, with dynamic trails, camera control, and interactive body creation.

---

## 🎬 Demo

https://github.com/user-attachments/assets/2a1784cf-d781-4cb7-a8be-5f285b1324cb

---

## 🚀 Features

- 🌍 **Realistic N-body simulation** using Newton's law of universal gravitation.  
- 🧮 **RK4 (Runge–Kutta 4th Order)** numerical integration for stable motion.  
- 🪐 **3D visualization** of celestial systems with dynamic trails.  
- 🖱️ **Interactive simulation** — click to spawn new bodies dynamically.  
- 🎨 **Colored trails** showing motion paths with fading transparency.  
- 🎥 **Free camera controls** (mouse look and scroll zoom).

---

## 🧠 Skills Learned & Technologies Used

### 🧩 Programming & Algorithms
- Implemented **object-oriented architecture** via a custom `NbodySimulation` class.
- Applied **Runge–Kutta 4th order integration (RK4)** to solve motion equations.
- Designed **pairwise gravitational force computation** with optimized combinations.
- Used **matrix and vector data structures** for physical state management.
- Practiced handling **floating-point precision** and **numerical stability**.

### 🎮 Graphics & Visualization
- Learned **Raylib 3D rendering** for real-time simulation.
- Implemented **camera transformation** using spherical coordinates.
- Created smooth **trail effects** with gradual fade.
- Designed visual representations of planetary motion.

### 🧰 Tools & Libraries
- **C++17 STL** — vectors, pairs, arrays, assertions, random generation.
- **Raylib** — graphics rendering and input handling.
- **Raymath** — vector and matrix math utilities.
- **g++ / CMake** — compilation and project management.

---

## 🕹️ Controls

| Action              | Key / Mouse  |
| :------------------ | :----------- |
| **Rotate camera**   | Move mouse   |
| **Zoom in/out**     | Scroll wheel |
| **Spawn new body**  | Left-click   |
| **Exit simulation** | Close window |

---

## 🌠 Simulation Details

* Each body stores:

  * Position `(x, y, z)`
  * Velocity `(vx, vy, vz)`
  * Mass, radius, and color
* Forces are computed pairwise using Newton’s law:
  [
  F = G \frac{m_1 m_2}{r^2}
  ]
* The **RK4 integrator** updates the system each frame for stable, realistic motion.
* Trails visualize recent positions using alpha fading for a glowing path effect.

---

## 📚 Default Setup

Simulates a simplified **solar system**:

| Body    | Distance | Mass      | Color        |
| :------ | :------- | :-------- | :----------- |
| Sun     | 0        | 1,989,000 | 🟡 Yellow    |
| Mercury | 31.95    | 0.33      | ⚫ Dark Gray  |
| Venus   | 54.1     | 4.87      | 🤍 Beige     |
| Earth   | 74.3     | 5.97      | 🔵 Blue      |
| Mars    | 113.95   | 0.642     | 🔴 Red       |
| Jupiter | 389.25   | 1900      | 🟠 Orange    |
| Saturn  | 716.5    | 568       | 🟡 Gold      |
| Uranus  | 1436     | 86.8      | 💙 Sky Blue  |
| Neptune | 2247.5   | 102       | 🔵 Dark Blue |

---

## 🧩 How to Test This Project

To make testing quick and simple, the easiest way to run this project is by using the official **Raylib Quickstart** template.

1. **Download the Raylib Quickstart project** from [(https://github.com/raylib-extras/raylib-quickstart) or use your preferred template (for example, the C++ one from `raylib/examples`).  
2. Make sure the quickstart project compiles and runs correctly first — this ensures Raylib is properly installed on your system.  
3. Once confirmed, **replace the template’s `main.cpp` file** with the `main.cpp` file from this repository.  
4. Rebuild and run the project. You should see the **3D N-body gravitational simulation** appear with interactive camera controls and particle trails.

> ⚠️ Due to some local configuration issues, this is currently the most reliable way to run the simulation.  
> It’s quick, easy, and guarantees all Raylib dependencies are correctly linked.

---

## 🏁 Summary

This project demonstrates understanding of:

* **Physics-based simulation** and numerical integration
* **C++ class design and algorithm implementation**
* **3D graphics programming** with Raylib
* **Performance-conscious coding** and simulation optimization

A showcase of **physics, programming, and visualization**, great for exploring computational astrophysics or simulation engines.

---

### 🧑‍🚀 Qclipsing - Me

