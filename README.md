
# 🌌 N-Body Simulation (C++ & Raylib)

This project is a **3D N-body gravitational simulation** written in **C++**, using the **Runge-Kutta (RK4)** integration method and rendered in real time with **Raylib**.  
It visually demonstrates how celestial bodies (like planets and stars) interact under Newtonian gravity, with dynamic trails, camera control, and interactive body creation.

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
| **Add new body**    | Left-click   |
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

## 🏁 Summary

This project demonstrates understanding of:

* **Physics-based simulation** and numerical integration
* **C++ class design and algorithm implementation**
* **3D graphics programming** with Raylib
* **Performance-conscious coding** and simulation optimization

A showcase of **physics, programming, and visualization**, great for exploring computational astrophysics or simulation engines.

---

### 🧑‍🚀 Qclipsing - Me

