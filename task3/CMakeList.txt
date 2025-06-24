cmake_minimum_required(VERSION 3.16)
project(SnakeGame)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(sfml REQUIRED system window graphics audio)

# Add executable
add_executable(SnakeGame
    main.cpp
    Game.cpp
    Snake.cpp
    Food.cpp
    GameStateManager.cpp
)

# Link SFML libraries
target_link_libraries(SnakeGame
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
)

# Copy assets to build directory
add_custom_command(TARGET SnakeGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SnakeGame>/assets
)

# Platform-specific settings
if(WIN32)
    # Copy SFML DLLs on Windows
    add_custom_command(TARGET SnakeGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-audio> $<TARGET_FILE_DIR:SnakeGame>
    )
endif()

# Set output directory
set_target_properties(SnakeGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)ml-system> $<TARGET_FILE_DIR:SnakeGame>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-window> $<TARGET_FILE_DIR:SnakeGame>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-graphics> $<TARGET_FILE_DIR:SnakeGame>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sf