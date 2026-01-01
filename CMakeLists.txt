cmake_minimum_required(VERSION 3.16)

project(SandGame C)

# Use C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ---- SDL2 paths (EDIT THESE) ----
set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SDL2/include")
set(SDL2_LIB_DIR     "${CMAKE_SOURCE_DIR}/SDL2/lib")

# Executable
add_executable(SandGame sand.c)

# Include SDL2 headers
target_include_directories(SandGame PRIVATE ${SDL2_INCLUDE_DIR})

# Link SDL2
if (MSVC)
    target_link_libraries(SandGame
        ${SDL2_LIB_DIR}/SDL2.lib
        ${SDL2_LIB_DIR}/SDL2main.lib
    )

    # Required for SDL on Windows
    target_link_options(SandGame PRIVATE /SUBSYSTEM:WINDOWS)

else()
    # Non-Windows fallback (optional)
    target_link_libraries(SandGame SDL2)
endif()

# Silence MSVC CRT warnings
if (MSVC)
    target_compile_definitions(SandGame PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
