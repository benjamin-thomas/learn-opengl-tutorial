cmake_minimum_required(VERSION 3.28)
set(CMAKE_C_STANDARD 11)

project(learn_opengl_tutorial C)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG "master"
        GIT_SHALLOW 1
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Sunday111/glad
        GIT_TAG "main"
        GIT_SHALLOW 1
)

FetchContent_MakeAvailable(glad)

set(SOURCES
        src/01_Getting-started/Hello-Triangle/lesson.c
        src/01_Getting-started/Hello-Triangle/ex1.c
        src/01_Getting-started/Hello-Triangle/ex2.c
        src/01_Getting-started/Hello-Triangle/ex2b.c
        src/01_Getting-started/Hello-Triangle/ex3.c
        src/01_Getting-started/Shaders/a/lesson.c
        src/01_Getting-started/Shaders/b/lesson.c
        src/01_Getting-started/Shaders/c/lesson.c
)

foreach (SOURCE ${SOURCES})
    # Extract the relative path and executable name
    get_filename_component(RELATIVE_PATH ${SOURCE} DIRECTORY) # Get directory path of the source file
    set(PREFIX ${RELATIVE_PATH})
    get_filename_component(BASENAME ${SOURCE} NAME_WE)
    string(REPLACE "src/" "" PREFIX ${PREFIX}) # Remove "src/" prefix
    string(REPLACE "/" "__" TARGET_NAME ${PREFIX}.${BASENAME}) # Replace slashes with underscores for unique target name

    # Create the executable
    add_executable(${TARGET_NAME} ${SOURCE})
    target_compile_definitions(${TARGET_NAME} PRIVATE SUB_PROJECT_DIR="${CMAKE_SOURCE_DIR}/${RELATIVE_PATH}")
    
    target_link_libraries(${TARGET_NAME} PUBLIC glfw glad)

    # Turn on or off via CMake's build profiles (Debug by default in CLion)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(${TARGET_NAME} PROPERTIES
                COMPILE_FLAGS "-Wall -Wextra -Wpedantic -Werror"
        )
        target_compile_options(${TARGET_NAME} PRIVATE
                -fsanitize=address -fsanitize=undefined
        )
        target_link_options(${TARGET_NAME} PRIVATE
                -fsanitize=address -fsanitize=undefined
        )
    endif ()


endforeach ()