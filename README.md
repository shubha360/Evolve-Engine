# Kolorob-Engine

This is a C++ static library to build simple 2D games. This engine is currently under development. It uses **SDL2** as its windowing API and **OpenGL 3.3** as its graphics API.

### Currently Supported Features

- Necessary 2D game features like image loading, input processing, 2D camera, etc.

- FPS counting and limiting.

- Font loading from font files (ttf, otf, etc) and 16 x 16 bitmap font files.

- Simple GUI creation (buttons and plain text).

### Dependencies

- SDL 2.28.3

- GLEW 2.1.0

- GLM 1.0.0

- FreeType 2.13.2

### How To Use

***NOTE:*** The engine folder should be located one directory back from where the main project source files are located. An example could be like this - 

    - project dir
      - include dir
      - src dir (source files are here)
      - Kolorob-Engine

This should be built and used like a normal C++ static library without any precompiled headers. The dependencies stated above should already be installed in the system.

Contact me through my mail ***shubha360@outlook.com*** if you're having trouble using it. I intend to convert it to a CMake project in the future.

### Games Created Using This Engine
