#include <GLFW/glfw3.h>
#include <stdio.h>

void on_window_resize(GLFWwindow *, const int width, const int height) {
    glViewport(0, 0, width, height);
}

void handle_key_press(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//     if (action == GLFW_PRESS) {
//         printf("Key pressed: %d (scancode: %d)\n", key, scancode);
//     }
// }

int main(void) {
    glfwInit();

    /*
      To find the latest openGL version supported by your hardware, run:
         glxinfo | fgrep 'core profile version'
  
      We use v3.3 for this tutorial
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /*
    Telling GLFW we want to use the core-profile means we'll get access to a
    smaller subset of OpenGL features without backwards-compatible features we no
    longer need.
     */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window) {
        printf("Could not create a GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, on_window_resize);
    // glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        handle_key_press(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
