// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <stdio.h>

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}";

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
    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };
    // clang-format on

    // To later allocate memory on the GPU, we first want to reserve an "id"
    // It's also possible to generate an array of IDs.
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    // "Tag" this ID. This allows future calls to GL_ARRAY_BUFFER to reference the underlying memory.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Actually allocate memory on the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Shader step 1: we create a shader object
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Shader step 2: we attach the shader source, then dynamically compile the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Shader step 3: check the shader compilation status
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(vertexShader, sizeof(info_log), NULL, info_log);
        printf("Shader compilation failed! (err=%s\n)", info_log);
    }

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
