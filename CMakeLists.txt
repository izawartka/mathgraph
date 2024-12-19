# Minimum required version of CMake
cmake_minimum_required(VERSION 3.15)

# Project name
project(mathgraph)

if(WIN32)
    # !!!!! important !!!!!
    # For building on Windows, set this to the path of your vcpkg installation
    include(C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories("src")

# Collect all .cpp files in the src directory and its subdirectories
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add executable
add_executable(mathgraph ${SOURCES} )

if(WIN32) # Windows
    # Find and link SDL2
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(mathgraph
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )

    # Find and link libpng
    find_package(PNG CONFIG REQUIRED)
    target_link_libraries(mathgraph
		PRIVATE
		$<IF:$<TARGET_EXISTS:PNG::PNG>,PNG::PNG,PNG::PNG-static>
	)

    # Find and link SDL2_image
    find_package(SDL2_image CONFIG REQUIRED)
    target_link_libraries(mathgraph
        PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    )

    # Find and link SDL2_ttf
    find_package(SDL2_ttf CONFIG REQUIRED)
    target_link_libraries(mathgraph
        PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    )
    
    # Find and link spdlog
    find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(mathgraph PRIVATE spdlog::spdlog)

	add_definitions(-Dmain=SDL_main)
else() # Linux
    # Find and link SDL2
    find_package(SDL2 REQUIRED)
    target_link_libraries(mathgraph
        PRIVATE
        SDL2
        SDL2main
    )

    # Find and link SDL2_image
    find_package(SDL2_image REQUIRED)
    target_link_libraries(mathgraph
        PRIVATE
        SDL2_image
    )

    # Find and link SDL2_ttf
    find_package(SDL2_ttf REQUIRED)
    target_link_libraries(mathgraph
        PRIVATE
        SDL2_ttf
    )

    # Find and link spdlog
    find_package(spdlog REQUIRED)
    target_link_libraries(mathgraph PRIVATE spdlog)

    # Find and link fmt
    find_package(fmt REQUIRED)
    target_link_libraries(mathgraph PRIVATE fmt::fmt)
endif()

# Set the output directory for the executable
set_target_properties(mathgraph PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
