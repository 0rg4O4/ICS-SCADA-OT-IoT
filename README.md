# 🚀 Autonomous Missile Guidance and Interception Simulation System ( Conceptual Engineering)

## 📖 Project Overview

This project is an **advanced autonomous missile guidance simulation** written in C++. Unlike traditional 6-DOF simulations with predetermined paths, this system features:

- **Real-time autonomous target tracking** - The missile continuously updates its trajectory based on the target's CURRENT position
- **Randomized target behavior** - The target moves unpredictably, changing direction and velocity randomly
- **Fully randomized starting positions** - Both missile and target spawn at random locations each simulation
- **Intelligent guidance logic** - Dynamic turn rates based on distance to prevent overshoot
- **3D visualization** - Python-based visualization showing trajectories, miss distance, and speed profiles

This simulation demonstrates practical missile guidance principles including pure pursuit, lead computation, and proximity fuzing.

## 🎯 Features

### Core Capabilities
- ✅ **Real-time autonomous guidance** - Missile tracks target's current position every frame
- ✅ **Randomized target AI** - Target changes velocity randomly every 5-8 seconds
- ✅ **6-DOF Physics** - Full 3D motion with thrust, drag, gravity, and mass modeling
- ✅ **Distance-based turn rates** - Aggressive turning when far, gentle when close (prevents overshoot)
- ✅ **Speed limiting** - Maximum speed of Mach 3.5 (1200 m/s) for realistic performance
- ✅ **Randomized spawn positions** - Both entities spawn randomly within defined bounds

### Visualization Features
- 🎬 **3D trajectory plot** - See missile vs target paths in 3D space
- 📉 **Miss distance over time** - Track intercept progress
- ⚡ **Speed profile** - Monitor missile velocity throughout flight
- 🔭 **Top-down view** - XY projection for tactical analysis

### Simulation Modes
- **Fast mode** - Run simulations quickly for statistical analysis
- **Real-time mode** - Watch the intercept unfold with 0.05s timesteps

## 🏗️ Project Structure

Main Folder 
|-missile_sim.cpp
|-visualize.py

compile via g++ : g++ -std=c++11 -O2 -o missile missile_sim.cpp -pthread

run file : missile.exe

output will be stored in : flight_data.csv

run : python visualize.py ( make sure matplotlib is available)

<img width="1370" height="715" alt="Figure_1" src="https://github.com/user-attachments/assets/16346da6-60d8-4ae0-9062-f7e05ce6c4ce" />

<img width="724" height="717" alt="Screenshot 2026-03-30 222911" src="https://github.com/user-attachments/assets/9564dd51-1602-45e8-8c52-4dc1becd1fd0" />

<img width="724" height="679" alt="Screenshot 2026-03-30 222922" src="https://github.com/user-attachments/assets/db68c58f-220f-47fd-ae15-fa943565ed59" />
