# Pinhole Camera Model Simulation

![C++](https://img.shields.io/badge/C++-17-blue)
![Eigen](https://img.shields.io/badge/Library-Eigen3-orange)
![OpenCV](https://img.shields.io/badge/Library-OpenCV-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

This project is a C++ implementation of a 3D-to-2D projection engine. It focuses on the mathematical foundations of the Pinhole Camera Model, developed as a first-principles exercise for autonomous systems and robotics.

## Key Features
- **Mathematical Core**: Direct implementation of Intrinsic ($K$) and Extrinsic ($R, t$) matrices via Eigen.
- **Decoupled Architecture**: Abstract Visualizer interface for API-independent rendering.
- **Dynamic Orbit**: Real-time camera transformation and perspective simulation.

## Technical Stack
- **C++17**: Utilizing for memory management.
- **Eigen3**: Linear algebra operations.
- **OpenCV**: Visualization backend.
- **CMake**: Build automation.

## Simulation Preview
![Pinhole Camera Model Preview](media/demo_orbit.gif)

## Usage
```bash
mkdir build && cd build
cmake .. && make
./Pinhole-Camera-Model --graphic-engine opencv
```

## Roadmap
- [x] Core Pinhole Engine & OpenCV Integration (2026-02-06)
- [x] 3D Model Loading (Assimp integration for .obj/.fbx) (2026-02-07)
- [ ] Manual Orbit Control (Interactive camera movement)
- [ ] Basic Shading (Face and light calculations)
- [ ] Alternative Renderers (OpenGL/Vulkan implementations)

## License
Licensed under the MIT License.
