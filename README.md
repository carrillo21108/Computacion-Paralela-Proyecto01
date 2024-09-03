
# Fireworks Simulation with OpenMP Parallelization

This project simulates a fireworks display using the SDL2 library and OpenMP for parallel processing. The simulation includes rockets that launch and explode into particles, with parallelization applied to the particle generation and rendering for performance optimization.

## Features
- Simulates rockets that explode into particles.
- Parallelized particle generation, updating, and rendering using OpenMP.
- Customizable number of particles, particle life, OpenMP schedule type, and chunk size through command-line arguments.
- Simple FPS display for performance monitoring.
  
## Prerequisites
To compile and run this project, you need:
1. **SDL2**: Simple DirectMedia Layer library.
2. **OpenMP**: Parallel computing extension for C++.
3. **C++ Compiler**: Supporting OpenMP and SDL2 (e.g., `g++`).

### Install SDL2 on Linux (Ubuntu/Debian):
```bash
sudo apt-get install libsdl2-dev
```

### Install SDL2 on MacOS:
```bash
brew install sdl2
```

### Install SDL2 on Windows:
Follow the [SDL2 installation guide for Windows](https://wiki.libsdl.org/Installation).

## Build Instructions

1. **Clone the repository**:
    ```bash
    git clone <repository-url>
    cd <project-directory>
    ```

2. **Compile the project**:
    ```bash
    g++ -o fireworks fireworks.cpp -lSDL2 -fopenmp
    ```

3. **Run the executable**:
    ```bash
    ./fireworks <numParticles> <particleLife> <scheduleType> <chunkSize>
    ```
   - `<numParticles>`: Number of particles generated when a rocket explodes.
   - `<particleLife>`: Lifespan of each particle.
   - `<scheduleType>`: OpenMP schedule type (`static`, `dynamic`, or `guided`).
   - `<chunkSize>`: Chunk size for scheduling in OpenMP.

### Example:
```bash
./fireworks 100 500 static 10
```
This command runs the simulation with 100 particles per explosion, 500 frames of life for each particle, a `static` OpenMP schedule, and a chunk size of 10.

## Command-Line Arguments

| Argument        | Description                                                       |
| --------------- | ----------------------------------------------------------------- |
| `<numParticles>`| Number of particles per explosion.                                |
| `<particleLife>`| Life of each particle in frames.                                  |
| `<scheduleType>`| OpenMP scheduling type: `static`, `dynamic`, or `guided`.         |
| `<chunkSize>`   | Size of each chunk for parallel work distribution in OpenMP.      |

### OpenMP Scheduling Types
- **`static`**: Partitions loop iterations into chunks of fixed size.
- **`dynamic`**: Dynamically allocates chunks to threads as threads finish work.
- **`guided`**: Similar to `dynamic`, but chunk sizes decrease over time.

## Project Structure

```
.
├── fireworks.cpp        # Main source code file
└── README.md            # This README file
```

## How It Works

1. **Rocket Launch**: Rockets are generated at random positions and launch upwards.
2. **Explosion**: When a rocket reaches its explosion height, it explodes into multiple particles.
3. **Parallelization**:
   - Particle creation, updating, and rendering are parallelized using OpenMP.
   - You can control the parallelization behavior using different scheduling techniques (`static`, `dynamic`, `guided`) and chunk sizes.
4. **Rendering**: Each particle has a random color and bounces off the screen edges until its life ends.
5. **FPS Monitoring**: The program shows the frames per second (FPS) in the window title.

## Performance Considerations
- The **number of particles** and their **life** directly affect the complexity and performance.
- Choosing an appropriate **OpenMP schedule type** and **chunk size** is crucial for balancing the workload between threads and improving performance.

## License
This project is released under the MIT License.

## Contributing

- Brian Carrillo 21108
- Carlos Lopez 21666
- Diego Alonzo 20172


## Contact
For any questions or suggestions, please contact the project maintainer.

Enjoy the fireworks simulation! 🎆
