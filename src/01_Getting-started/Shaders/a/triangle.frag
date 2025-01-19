#version 330 core

out vec4 frag_color;

in vec4 vertex_color;  // from the vertex shader

void main() {
    frag_color = vertex_color;
}