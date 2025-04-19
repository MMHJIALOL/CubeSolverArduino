# 🧊 Rubik's Cube Solver with Arduino 

This is a Rubik’s Cube solving robot powered by **Arduino Mega 2560**, using **RAMPS 1.4**, **A4988 drivers**, **NEMA 17 stepper motors**, and a **16x2 LCD** to display real-time timing. It can solve a Rubik's Cube of any scrambled state in exactly 20 moves under 2 seconds and gives feedback of time taken via the display.

---

### 🧰 Hardware Components

### 🔌 Microcontroller & Motor Control
- Arduino Mega 2560
- RAMPS 1.4 Board
- 6× A4988 Stepper Motor Drivers
  - 5 via RAMPS slots (X, Y, Z, E0, E1)
  - 1 via breadboard extension with external wiring

### ⚙️ Stepper Motors
- 6× NEMA 17 stepper motors (1 per Rubik's Cube face)
- Controlled using A4988 drivers

### 🔋 Power Supply
- 12V, 5A Power Supply
- Common ground shared between all systems

### 📺 Display
- 16x2 LCD (HD44780 Parallel Interface)
- Displays solving status, real-time elapsed time, and completion message.

---

## 🔌 Wiring Overview

![Wiring Diagram](fritzingsketch.png)

> Full connection diagram created using Fritzing. The last two motors are connected via breadboard using external A4988 drivers.
---

## 🧠 Software Components

### 🧾 Arduino Sketch
- Written in C++ using the Arduino IDE
- Uses the standard `LiquidCrystal` library
- Timer starts on motion execution and updates every step
- Shows final time with animation

### 🧱 Code Features
- `Arduino/`: Full Arduino code for controlling motors and executing the moves
- `OpenCV/`: Color detection script
- `Koceimba/`: Cube solver algorithm 
- `SolverAlgo/`: Code that parses the detected colors and applies the solver

---
## 🙏 Credits
- OpenCV color detection: [Reference Repo]([https://github.com/username/color-detector](https://github.com/prahalad12345/cubesolver/blob/main/colorreader.py))  
- Koceimba solver: [Reference Repo]([https://github.com/username/koceimba-solver](https://github.com/prahalad12345/cubesolver/blob/main/solver.py))
- 3D printed parts were inspired by online models

## 🎥 Demo
[![IMAGE ALT TEXT HERE](https://github.com/user-attachments/assets/ebeb1fb9-0243-45b7-8648-3a207db50d76)](https://www.youtube.com/shorts/S8Q5pxlBp1k)

## 📜 License

MIT for all code written/modified by me and @manan511.

---

