cmake_minimum_required(VERSION 3.30)
project(learn_opengl_tutorial C)

set(CMAKE_C_STANDARD 11)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
)

#[[

For Linux users compiling with GCC, the following command line options may help you compile the project:
 
 -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl. 
 
Not correctly linking the corresponding libraries will generate many undefined reference errors.
]]

add_executable(learn_opengl_tutorial src/glad.c src/main.c)
target_link_libraries(learn_opengl_tutorial glfw GL X11 pthread Xrandr Xi dl)
