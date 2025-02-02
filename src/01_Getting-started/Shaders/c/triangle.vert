#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

out vec3 our_color; // output color for the fragment shader

void main() {
    gl_Position = vec4(pos, 1.0);
    our_color =  color; // assign the color from the VAO
}