# Classical RPG Mechanics

Testing bed for testing classic RPG mechanics / algorithms in a text-based environment. These are used to explore and familiarize the concepts of the various algorithms. These include things such as movement in a text-based environment, random generation of content, pathfinding, flocking, manipulating environments, etc.

This project is targeted at C++ 20.

## Compiling / Running

This project uses CMake, after cmake is installed:

```bash
# Create the build environment.
mkdir -p build
cd build/
cmake ..

# Build the project.
make

# Run the application.
./rpg
```

## Algorithms and Elements

### Controllers

- Game Tick (Tick Controller)

### Terrain Generation

- Wave Function Collapse (Simple Tiled Model)
