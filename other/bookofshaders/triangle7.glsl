#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    uv.y += 0.25;

    float y = 1.0 - abs(uv.x - 0.5);

    vec3 color =
        (uv.y >= 0.5 && uv.y <= y) ? vec3(1.0, 0.0, 0.0) : vec3(0.0, 0.0, 0.0);

    gl_FragColor = vec4(color, 1.0);
}
