#version 330 core

out vec4 frag_color;

uniform vec4 u_color; // value injected via C

void main() {
    frag_color = u_color;
}