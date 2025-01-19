// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600

enum failures {
    FILE_OPEN_FAILED = 1,
    ALLOC_FAILED = 2,
    READ_FAILED = 3,
    COMPILATION_FAILED = 4
};

static int read_file_err(const char *path, char **source_out) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Could not open file: %s\n", path);
        return FILE_OPEN_FAILED;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    const size_t len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate
    *source_out = malloc(len + 1); // +1 for null terminator
    if (!*source_out) {
        printf("Could not allocate memory for file contents\n");
        fclose(file);
        return ALLOC_FAILED;
    }

    // Read
    const size_t read = fread(*source_out, sizeof(char), len, file);
    if (read != len) {
        printf("Could not read file contents\n");
        fclose(file);
        return READ_FAILED;
    }

    // Add null terminator
    (*source_out)[len] = '\0';

    fclose(file);
    return 0;
}

static GLuint must_make_vertex_shader(const char *source) {
    const GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        printf("Vertex shader compilation failed! (err=%s\n)", info_log);
        exit(1);
    }
    return shader;
}

static GLuint must_make_fragment_shader(const char *source) {
    const GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, (const GLchar * const*) &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        printf("Fragment shader compilation failed! (err=%s\n)", info_log);
        exit(1);
    }

    return shader;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void on_window_resize(GLFWwindow *window, const int width, const int height) {
    (void) window;
    glViewport(0, 0, width, height);
}

#define MODES_LEN 2
int modes[MODES_LEN] = {
    GL_FILL,
    GL_LINE,
};
int mode_idx = 0;

static void handle_key_press(GLFWwindow *window) {
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

int main(void) {
#ifdef __SANITIZE_ADDRESS__
    printf("AddressSanitizer is activated!\n");
#else
    printf("AddressSanitizer is NOT activated.\n");
#endif

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window =
            glfwCreateWindow(WIDTH, HEIGHT, "Shaders Lesson (multi layout)", NULL, NULL);
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

    int max_vertex_attribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribs);

    printf("GL_MAX_VERTEX_ATTRIBS: %d\n", max_vertex_attribs);

    char *vertex_shader_source;
    if (read_file_err(SUB_PROJECT_DIR "/triangle.vert",
                      &vertex_shader_source)) {
        return -1;
    }
    const GLuint vertex_shader = must_make_vertex_shader(vertex_shader_source);

    char *fragment_shader_source;
    if (read_file_err(SUB_PROJECT_DIR "/triangle.frag",
                      &fragment_shader_source)) {
        return -1;
    }
    const GLuint fragment_shader =
            must_make_fragment_shader(fragment_shader_source);

    // Build the shader program
    const unsigned int shader_program = glCreateProgram(); {
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
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    // Create the buffers
    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // clang-format off
    const float vertices[18] = {
        //  [positions]              [colors]
        0.0f,  0.8f, 0.0f,      1.0f, 0.0f, 0.0f,  // t
       -0.8f, -0.8f, 0.0f,      0.0f, 1.0f, 0.0f,  // bl
        0.8f, -0.8f, 0.0f,      0.0f, 0.0f, 1.0f,  // br
    };
    // clang-format on

    // Use the buffers
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shader_program);

    while (!glfwWindowShouldClose(window)) {
        handle_key_press(window);
        glPolygonMode(GL_FRONT_AND_BACK, modes[mode_idx]);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}
