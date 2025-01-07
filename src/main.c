// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <stdio.h>

// ReSharper disable once CppParameterNeverUsed
void on_window_resize(GLFWwindow *_window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

void handle_key_press(GLFWwindow *window) {
    const int escape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    const int caps_lock = glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS;
    if (escape || caps_lock) {
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

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window) {
        printf("Could not create a GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, on_window_resize);
    // glfwSetKeyCallback(window, key_callback);

    // ReSharper disable once CppRedundantCastExpression
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return 1;
    }
    glViewport(0, 0, 800, 600);

    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };
    // clang-format on

    unsigned int vbo;
    glGenBuffers(1, &vbo);

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
