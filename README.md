# mathgraph
An intuitive graphical calculator leveraging the latest features of the RZUF framework and an efficient custom mathematical expression parser. It supports algebraic expressions, shorthand notation (e.g., `x(x-4)`), and basic mathematical functions like `sin`, `cos`, `log`, etc.

### Planned features:
- Error highlighing in mathematical expressions
- Precise value reading at a specific point (using the mouse)
- Logarithmic scale
- π-based scale
- Configuration file support
- Improved rendering accuracy (addressing issues with asymptotes)

### RZUF3
You can find the framework repository [here](https://github.com/izawartka/rzuf3).

### Binaries
The latest binaries are available in the Releases section on the right. Just download the zip file, extract it, and run `mathgraph.exe`.

### Dependencies
RZUF3 requires the following libraries: `SDL2`, `SDL2_image`, `SDL2_ttf`, `spdlog`. For installation on Windows, it's best to use vcpkg. If CMake has trouble finding them, ensure the appropriate path is set in the second line of `CMakeLists.txt`.

### Author
masuo / izawartka
