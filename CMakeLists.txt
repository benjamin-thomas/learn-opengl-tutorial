cmake_minimum_required(VERSION 3.28)
set(CMAKE_C_STANDARD 11)

project(learn_opengl_tutorial C)
set(target_name "learn_opengl_tutorial")
add_executable(${target_name} src/main.c)


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

target_link_libraries(${target_name} PUBLIC glfw glad)
