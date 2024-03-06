# Evolve-Engine

This is a C++ static library to build simple 2D games. This engine is currently under development. It uses **SDL2** as its windowing API and **GLEW** to access **OpenGL 3.3** as its graphics API.

### Currently Supported Features

- Necessary 2D game features like image loading, input processing, 2D camera, etc.

- FPS counting and limiting.

- Font loading from font files (ttf, otf, etc) and 16 x 16 bitmap font files.

- Simple GUI creation (buttons and plain text).

### Dependencies

- SDL v2.28.3

- GLEW v2.1.0

- GLM v1.0.0

- FreeType v2.13.2

- stb_image v2.29

### How To Use

This can be used in one of 2 ways - 

- By copying all the code files into the main project. This approach prevents complexity but makes the project messy with many files.

- By building this as a C++ static library without any precompiled headers. This approach will prevent unnecessary messiness.

Either way, the dependencies stated above should already be installed in the system.

Contact me through my mail ***shubha360@outlook.com*** if you're having trouble using it. I intend to convert it to a CMake project in the future.

### Games Created Using This Engine
