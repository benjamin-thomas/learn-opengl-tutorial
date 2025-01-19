package main

import gl "vendor:OpenGL"
import "vendor:glfw"

// This load macro is very handy!
// I may have to load the shaders at runtime later to support hot-reloading
// NOTE: there's a `load_shaders_file` function in helpers.odin
VERT_SRC :: string(#load("triangle.vert"))
FRAG_SRC :: string(#load("triangle.frag"))

modes := [2]u32{ gl.FILL, gl.LINE }
modes_idx := 0

/*
odin check .
odin build . -debug -sanitize:address
odin run . -use-separate-modules
*/
main :: proc() {
    glfw.Init()
    defer glfw.Terminate()

    glfw.WindowHint(glfw.CONTEXT_VERSION_MAJOR, 3)
    glfw.WindowHint(glfw.CONTEXT_VERSION_MINOR, 3)
    glfw.WindowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

    when ODIN_OS == .Darwin {
        glfw.WindowHint(glfw.OPENGL_FORWARD_COMPAT, gl.TRUE)
    }

    window := glfw.CreateWindow(800, 600, "Shaders Lesson", nil, nil)
    if window == nil {
        panic("Failed to create window")
    }
    defer glfw.DestroyWindow(window)

    glfw.MakeContextCurrent(window)
    glfw.SetFramebufferSizeCallback(window, on_window_resize)
    gl.load_up_to(3, 3, glfw.gl_set_proc_address) // Load OpenGL function pointers

    // Display on the second monitor, if available.
    set_window_position(window)

    pgm, ok := gl.load_shaders_source(VERT_SRC, FRAG_SRC)
    if !ok {
        panic("Shader compilation failed")
    }
    defer gl.DeleteProgram(pgm)

    // @formatter:off
    vertices := [9]f32 {
        -0.0,  0.8, 0.0, // t
        -0.8, -0.8, 0.0, // bl
         0.8, -0.8, 0.0, // br
    }
    // @formatter:on
    indices := [3]u32{ 0, 1, 2 }

    // Create the buffers
    vao, vbo, ebo: u32
    gl.GenVertexArrays(1, &vao)
    gl.GenBuffers(1, &vbo)
    gl.GenBuffers(1, &ebo)
    defer gl.DeleteVertexArrays(1, &vao)
    defer gl.DeleteBuffers(1, &vbo)
    defer gl.DeleteBuffers(1, &ebo)

    gl.BindVertexArray(vao)
    gl.BindBuffer(gl.ARRAY_BUFFER, vbo)
    gl.BufferData(gl.ARRAY_BUFFER, size_of(vertices), &vertices[0], gl.STATIC_DRAW)

    gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, ebo)
    gl.BufferData(gl.ELEMENT_ARRAY_BUFFER, size_of(indices), &indices[0], gl.STATIC_DRAW)

    gl.VertexAttribPointer(0, 3, gl.FLOAT, gl.FALSE, size_of([3]f32), 0)
    gl.EnableVertexAttribArray(0)

    for !glfw.WindowShouldClose(window) {
        handle_key_press(window)
        gl.PolygonMode(gl.FRONT_AND_BACK, modes[modes_idx])

        gl.ClearColor(0.2, 0.3, 0.3, 1.0)
        gl.Clear(gl.COLOR_BUFFER_BIT)

        gl.UseProgram(pgm)
        gl.BindVertexArray(vao)
        gl.DrawElements(gl.TRIANGLES, 3, gl.UNSIGNED_INT, nil)

        glfw.SwapBuffers(window)
        glfw.PollEvents()
    }
}

set_window_position :: proc(window: glfw.WindowHandle) {
    monitors := glfw.GetMonitors()
    target_monitor := len(monitors) > 1 ? monitors[1] : monitors[0]
    mode := glfw.GetVideoMode(target_monitor)
    x_pos, y_pos := glfw.GetMonitorPos(target_monitor)
    glfw.SetWindowPos(window, x_pos + mode.width / 4, y_pos + mode.height / 4)
}

on_window_resize :: proc "c" (window: glfw.WindowHandle, width: i32, height: i32) {
    gl.Viewport(0, 0, width, height)
}

Key_State :: struct {
    was_space_pressed: bool,
}
global_key_state := Key_State {
    was_space_pressed = false,
}

handle_key_press :: proc(window: glfw.WindowHandle) {
    is_caps_lock_pressed := glfw.GetKey(window, glfw.KEY_CAPS_LOCK) == glfw.PRESS
    is_escape_pressed := glfw.GetKey(window, glfw.KEY_ESCAPE) == glfw.PRESS
    is_space_pressed := glfw.GetKey(window, glfw.KEY_SPACE) == glfw.PRESS

    if (is_escape_pressed || is_caps_lock_pressed) {
        glfw.SetWindowShouldClose(window, true)
    }

    if (is_space_pressed) {
        if (!global_key_state.was_space_pressed) {
            modes_idx = (modes_idx + 1) % len(modes)
            global_key_state.was_space_pressed = true
        }
    } else {
        global_key_state.was_space_pressed = false
    }
}
