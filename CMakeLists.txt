cmake_minimum_required(VERSION 3.28)
set(CMAKE_C_STANDARD 11)

project(learn_opengl_tutorial C)
#set(target_name "learn_opengl_tutorial")
#add_executable(${target_name} src/main.c)


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
)

#foreach(SOURCE ${SOURCES})
#    get_filename_component(EXECUTABLE ${SOURCE} NAME_WE) # Extract base name of the source file
#    get_filename_component(DIR ${SOURCE} DIRECTORY) # Get relative path of source file
#    string(REPLACE "src/" "" REL_DIR ${DIR}) # Remove "src/" prefix
#
#    add_executable(${EXECUTABLE} ${SOURCE})
#    target_link_libraries(${EXECUTABLE} PUBLIC glfw glad)
#
#    # Set the runtime output directory to mirror the source directory structure
#    set_target_properties(${EXECUTABLE} PROPERTIES
#            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${REL_DIR}"
#    )
#endforeach()

# Loop over the sources
foreach(SOURCE ${SOURCES})
    # Extract the relative path and executable name
    get_filename_component(RELATIVE_PATH ${SOURCE} DIRECTORY) # Get directory path of the source file
    get_filename_component(BASENAME ${SOURCE} NAME_WE)
    string(REPLACE "src/" "" RELATIVE_PATH ${RELATIVE_PATH}) # Remove "src/" prefix
    string(REPLACE "/" "__" TARGET_NAME ${RELATIVE_PATH}.${BASENAME}) # Replace slashes with underscores for unique target name

    # Create the executable
    add_executable(${TARGET_NAME} ${SOURCE})
    target_link_libraries(${TARGET_NAME} PUBLIC glfw glad)
endforeach()