# mathgraph
An intuitive graphical calculator leveraging the latest features of the RZUF framework and an efficient custom mathematical expression parser. It supports algebraic expressions, shorthand notation (e.g., `x(x-4)`), and some mathematical functions like `sin`, `cos`, `log`, `asinh`, `abs`, `mod`, etc. User can choose between linear (decimal), logarithmic, and pi-based scales for the X and Y axes, as well as preview function values at specific points by hovering over the graph with the cursor. The tool is available in both English and Polish.

![Example expression](https://maseuko.pl/res/mathgraph.png)

### Planned features:
- Rendering multiple expressions at once
- Improved error highlighing in mathematical expressions
- Improved rendering accuracy (addressing issues with asymptotes)

### RZUF3
You can find the framework repository [here](https://github.com/izawartka/rzuf3).

### Binaries
The latest binaries are available in the Releases section on the right. Just download the zip file, extract it, and run `mathgraph.exe`.

### Dependencies
RZUF3 requires the following libraries: `SDL2`, `SDL2_image`, `SDL2_ttf` (min. >=2.0.18, recommended >=2.20.0), `spdlog`.

### Windows building
1. Make sure you have [Visual Studio](https://visualstudio.microsoft.com/) (with CMake support and vcpkg) installed.

2. Install the necessary packages:  
   `vcpkg install sdl2 sdl2-image sdl2-ttf spdlog`

3. Clone the repo using the command:  
    `git clone https://github.com/izawartka/mathgraph`  
    or just download the ZIP archive from the repository page.

4. Ensure that a proper path to the vcpkg toolchain file is set in the `CMakeLists.txt` file. Default is `C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake`.

5. Open the Cmake project in Visual Studio, wait for CMake to configure the project, and then build it using the `x64-Release` or `x64-Debug` configuration.

6. Done! The binary file should be located in the `bin` directory.

### Linux building
1. Install the necessary packages. For Debian-based distributions, use the command:  
   `sudo apt install git cmake gcc ninja-build libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libspdlog-dev -y`

2. Clone the repo:  
   `git clone https://github.com/izawartka/mathgraph`.

3. Navigate to the directory:  
   `cd mathgraph`.

4. Build the project  
   For the release version:  
   `cmake --preset x64-Release`  
   `cmake --build --preset x64-Release`  

   For the debug version:  
   `cmake --preset x64-Debug`  
   `cmake --build --preset x64-Debug`  

5. Done! The binary file should be located in the `bin` directory.

### Author
masuo / izawartka
