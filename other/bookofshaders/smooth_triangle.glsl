#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform vec2 u_resolution;

float plot(vec2 pos) {
    // Scale x to [-1, 1] range as before
    float x = (pos.x - 0.5) * 2.0;
    float y = 1.0 - abs(x);

    // Use fwidth() to calculate how much anti-aliasing we need
    // fwidth() helps determine the size of a pixel at this position
    float thickness = 0.01;
    float delta =
        fwidth(y) * 1.0;  // Multiply by 1.0 to adjust anti-aliasing amount

    // Create sharp but anti-aliased line
    return 1.0 -
           smoothstep(thickness - delta, thickness + delta, abs(y - pos.y));
}

void main() {
    vec2 pos = gl_FragCoord.xy / u_resolution;

    float line = plot(pos);

    vec3 color = vec3(1.0, 0.0, 0.0);
    color += line;

    gl_FragColor = vec4(color, 1.0);
}