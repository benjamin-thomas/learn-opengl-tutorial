// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <stdio.h>


/*
Try to draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data.
 */

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

#define WIDTH 800
#define HEIGHT 600

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window) {
        printf("Could not create a GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, on_window_resize);

    // Display on the second monitor, if available.
    {
        int monitor_count;
        GLFWmonitor **monitors = glfwGetMonitors(&monitor_count);
        GLFWmonitor *target_monitor = monitor_count >= 2 ? monitors[1] : monitors[0];
        const GLFWvidmode *mode = glfwGetVideoMode(target_monitor);

        // Position the window on the target monitor
        int x_pos, y_pos;
        glfwGetMonitorPos(target_monitor, &x_pos, &y_pos);
        glfwSetWindowPos(window, x_pos + mode->width / 4, y_pos + mode->height / 4);
    }

    // Load all OpenGL function pointers
    // ReSharper disable once CppRedundantCastExpression
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return 1;
    }

    // Build the vertex shader
    const unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    {
        glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader);

        int success;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(vertex_shader, sizeof(info_log), NULL, info_log);
            printf("Vertex shader compilation failed! (err=%s\n)", info_log);
        }
    }

    // Build the fragment shader
    const unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    {
        glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

        int success;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL, info_log);
            printf("Fragment shader compilation failed! (err=%s\n)", info_log);
        }
    }

    // Build the shader program
    const unsigned int shader_program = glCreateProgram();
    {
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);

        int success;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
        }
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    // clang-format off
    const float vertices[] = {
        -0.45f, 0.5f, 0.0f, // t
        -0.8f, -0.5f, 0.0f, // bl
        -0.1f, -0.5f, 0.0f, // br
        
         0.45f,  0.5f, 0.0f, // t
         0.8f, -0.5f, 0.0f,  // br
         0.1f, -0.5f, 0.0f,  // bl
    };
    // clang-format on

    // 0. create the required buffers
    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // 1. bind the buffers
    glBindVertexArray(vao);
    // 2. copy the vertices into the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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

        // We divide by 3 since each vertex position is defined via 3 values (x,y,z)
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(int) / 3);
        glPolygonMode(GL_FRONT_AND_BACK, modes[mode_idx]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);
    glfwTerminate();
    return 0;
}
