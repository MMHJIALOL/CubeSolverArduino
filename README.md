# Rubik's Cube Solver Robot 🤖🧊

This is a Rubik's Cube solving robot project built using:
- **Arduino Mega + Stepper motors** to control a physical cube-turning robot
- **OpenCV** for webcam-based color detection (borrowed code)
- **Koceimba's cube solving algorithm** (used without changes)
- **Algo** to convert face colors into a valid cube state and solve it!

## 🧱 What's Inside

- `Arduino/`: Full Arduino code for controlling motors and executing the moves
- `OpenCV/`: Color detection script taken from another GitHub repo
- `Koceimba/`: Cube solver algorithm (original, untouched)
- `SolverAlgo/`: Custom code that parses the detected colors and applies the solver

## 🙏 Credits

- OpenCV color detection: [Reference Repo]([https://github.com/username/color-detector](https://github.com/prahalad12345/cubesolver/blob/main/colorreader.py))  
- Koceimba solver: [Reference Repo]([https://github.com/username/koceimba-solver](https://github.com/prahalad12345/cubesolver/blob/main/solver.py))
- 3D printed parts were inspired by online models

## 🎥 Demo

*Coming soon: a video of the robot in action!*

## 📜 License

MIT for all code written/modified by me. Original components retain their respective licenses.

---

