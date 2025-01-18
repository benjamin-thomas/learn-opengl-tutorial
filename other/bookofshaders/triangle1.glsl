#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;

void main() {
    vec2 uv = (gl_FragCoord.xy / u_resolution.xy) - 0.5;

    float y = abs(uv.x);

    vec3 color =
        (uv.y >= 0.0 && uv.y <= y) ? vec3(1.0, 0.0, 0.0) : vec3(0.0, 0.0, 0.0);

    gl_FragColor = vec4(color, 1.0);
}
