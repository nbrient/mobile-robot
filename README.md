# mobile-robot

First commit of a C++ / SFML project.

## Features
- Window creation
- 2D map rendering
- Random obstacle generation
- Configurable map size
- Configurable obstacle count
- JSON config loading

## Config
The application loads its configuration from:

```bash
config/config.json
```

## Build

```bash
mkdir build
cd build
cmake ..
make
./robot_manual_target
```
