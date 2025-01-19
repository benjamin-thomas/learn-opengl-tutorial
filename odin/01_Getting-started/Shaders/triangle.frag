#version 330 core

out vec4 frag_color;

in vec4 vertex_color;  // from the vertex shader
uniform vec4 u_color; // injected via the C code

void main() {
//    frag_color = vertex_color;
    frag_color = u_color;
}