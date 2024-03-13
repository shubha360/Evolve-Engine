# Evolve-Engine

This is a C++ static library to build simple 2D games. This engine is currently under development. It uses **SDL2** as its windowing API and **GLEW** to access **OpenGL 3.3** as its graphics API.

***Disclaimer:*** When considering performance, this engine might not offer the highest efficiency.

This project takes inspiration from these learning materials - [MakingGamesWithBen](https://www.youtube.com/@makinggameswithben/featured), [LazyFoo's Tutorials](https://lazyfoo.net/), [The Cherno](https://www.youtube.com/@TheCherno)

### Currently Supported Features

- Necessary 2D game features like image loading, input processing, 2D camera, etc.

- FPS counting and limiting.

- Font loading from font files (TTF, OTF, etc) and 16 x 16 bitmap font files.

- Simple GUI creation (buttons and plain text).

### Dependencies

- SDL 2.28.3

- GLEW 2.1.0

- opengl32

- GLM 1.0.0

- FreeType 2.13.2

- stb_image 2.29

### How To Use

For use in Visual Studio on both 32-bit and 64-bit Windows systems, you can download the pre-built library bundled with the additional dependencies from [here](https://github.com/shubha360/Evolve-Engine/releases).

Else, this can be used in one of 2 ways - 

- By copying all the code files into the main project. This approach prevents the complexity of building the library but makes the project messy with many files.

- By building this as a C++ static library without any precompiled headers. This approach will prevent unnecessary messiness.

Either way, the dependencies stated above should already be installed in the system.

Please contact me at ***shubha360@outlook.com*** if you're having trouble using it. I intend to convert it to a CMake project in the future.

### Games Created Using This Engine

- [Tetris](https://github.com/shubha360/Tetris_Recreated)

This project is licensed under the **[MIT License](LICENSE)**. You are welcome to make or propose any changes or improvements.
