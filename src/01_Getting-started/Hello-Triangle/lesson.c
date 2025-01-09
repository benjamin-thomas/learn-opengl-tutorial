// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <stdio.h>

const char *vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}";

const char *fragment_shader_source =
        "#version 330 core\n"
        "out vec4 frag_color;"
        "void main() {"
        "  frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
        "}";

// ReSharper disable once CppParameterNeverUsed
void on_window_resize(GLFWwindow *_window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

#define MODES_LEN 2
int modes[MODES_LEN] = {
    GL_FILL,
    GL_LINE,
};
int mode_idx = 0;

void handle_key_press(GLFWwindow *window) {
    static int was_space_pressed = 0;
    const int is_space_pressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    const int is_escape_pressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    const int is_caps_lock_pressed = glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS;

    if (is_space_pressed) {
        if (!was_space_pressed) {
            mode_idx = (mode_idx + 1) % MODES_LEN;
            was_space_pressed = 1;
        }
    } else {
        was_space_pressed = 0;
    }

    if (is_escape_pressed || is_caps_lock_pressed)
        glfwSetWindowShouldClose(window, 1);
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

    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
    if (!window) {
        printf("Could not create a GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, on_window_resize);

    // ReSharper disable once CppRedundantCastExpression
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return 1;
    }
    glViewport(0, 0, 800, 600);


    // Vertex shader step 1: we create a shader object
    const unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    // Vertex shader step 2: we attach the shader source, then dynamically compile the shader
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    // Vertex shader step 3: check the shader compilation status
    int vertex_success;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_success);
    if (!vertex_success) {
        char info_log[512];
        glGetShaderInfoLog(vertex_shader, sizeof(info_log), NULL, info_log);
        printf("Vertex shader compilation failed! (err=%s\n)", info_log);
    }

    // Fragment shader steps
    const unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    int fragment_success;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_success);
    if (!fragment_success) {
        char info_log[512];
        glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL, info_log);
        printf("Fragment shader compilation failed! (err=%s\n)", info_log);
    }

    // Now, we link the compiled shaders into a "shader program"
    const unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // And we check the linking status
    int link_success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &link_success);
    if (!link_success) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
    }

    // clang-format off
    const float vertices[] = {
        -0.5f,  0.5f, 0.0f, // top left
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
    };
    const unsigned int indices[] = {
        0, 1, 3, // first triangle: tl, tr, bl
        1, 3, 2 // second triangle: tr, bl, br 
    };
    // clang-format on

    // 0. create the required buffers
    unsigned int vao, ebo, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &vbo);

    // 1. bind the buffers
    glBindVertexArray(vao);
    // 2a. copy the vertices into the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 2b. copy the indices into the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 3. set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // Finalize the VAO setup (I think)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        handle_key_press(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the linked program
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);


        glPolygonMode(GL_FRONT_AND_BACK, modes[mode_idx]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
