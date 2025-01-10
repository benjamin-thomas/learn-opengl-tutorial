// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <stdio.h>

/*
Create two shader programs where the second program uses a different fragment 
shader that outputs the color yellow; draw both triangles again where one 
outputs the color yellow:
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

const char *fragment_shader2_source =
    "#version 330 core\n"
    "out vec4 frag_color;"
    "void main() {"
    "  frag_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);"
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
    const int is_space_pressed =
        glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    const int is_escape_pressed =
        glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    const int is_caps_lock_pressed =
        glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS;

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

    GLFWwindow *window =
        glfwCreateWindow(WIDTH, HEIGHT, "Hello Triangle: ex3", NULL, NULL);
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
        GLFWmonitor *target_monitor =
            monitor_count >= 2 ? monitors[1] : monitors[0];
        const GLFWvidmode *mode = glfwGetVideoMode(target_monitor);

        // Position the window on the target monitor
        int x_pos, y_pos;
        glfwGetMonitorPos(target_monitor, &x_pos, &y_pos);
        glfwSetWindowPos(window, x_pos + mode->width / 4,
                         y_pos + mode->height / 4);
    }

    // Load all OpenGL function pointers
    // ReSharper disable once CppRedundantCastExpression
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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
            glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL,
                               info_log);
            printf("Fragment shader compilation failed! (err=%s\n)", info_log);
        }
    }

    // Build the other fragment shader
    const unsigned int fragment_shader2 = glCreateShader(GL_FRAGMENT_SHADER);
    {
        glShaderSource(fragment_shader2, 1, &fragment_shader2_source, NULL);
        glCompileShader(fragment_shader2);

        int success;
        glGetShaderiv(fragment_shader2, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(fragment_shader2, sizeof(info_log), NULL,
                               info_log);
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
            glGetProgramInfoLog(shader_program, sizeof(info_log), NULL,
                                info_log);
        }
        // glDeleteShader(vertex_shader); // delete after second use
        glDeleteShader(fragment_shader);
    }

    // Build the other shader program
    const unsigned int shader_program2 = glCreateProgram();
    {
        glAttachShader(shader_program2, vertex_shader);
        glAttachShader(shader_program2, fragment_shader2);
        glLinkProgram(shader_program2);

        int success;
        glGetProgramiv(shader_program2, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetProgramInfoLog(shader_program2, sizeof(info_log), NULL,
                                info_log);
        }
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader2);
    }

#define TRIANGLES_CNT 2
    const float vertices[TRIANGLES_CNT][9] = {
        {
            -0.45f, 0.5f, 0.0f,  // t
            -0.8f, -0.5f, 0.0f,  // bl
            -0.1f, -0.5f, 0.0f,  // br
        },
        {
            0.45f, 0.5f, 0.0f,  // t
            0.8f, -0.5f, 0.0f,  // br
            0.1f, -0.5f, 0.0f,  // bl
        },
    };

    // Create the buffers
    unsigned int vao_arr[TRIANGLES_CNT], vbo_arr[TRIANGLES_CNT];
    glGenVertexArrays(TRIANGLES_CNT, vao_arr);
    glGenBuffers(TRIANGLES_CNT, vbo_arr);

    // Use the buffers
    for (int i = 0; i < TRIANGLES_CNT; i++) {
        glBindVertexArray(vao_arr[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_arr[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i],
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);
    }

    while (!glfwWindowShouldClose(window)) {
        handle_key_press(window);
        glPolygonMode(GL_FRONT_AND_BACK, modes[mode_idx]);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < TRIANGLES_CNT; i++) {
            if (i == 0) {
                glUseProgram(shader_program);
            } else {
                glUseProgram(shader_program2);
            }
            glBindVertexArray(vao_arr[i]);
            glDrawArrays(GL_TRIANGLES, 0,
                         sizeof(vertices[i]) / sizeof(float) / 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(TRIANGLES_CNT, vao_arr);
    glDeleteBuffers(TRIANGLES_CNT, vbo_arr);
    glDeleteProgram(shader_program);
    glDeleteProgram(shader_program2);
    glfwTerminate();
    return 0;
}
