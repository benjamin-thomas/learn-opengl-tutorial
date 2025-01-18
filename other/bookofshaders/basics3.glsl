/*
Viewing various shapes from:

    https://thebookofshaders.com/05/kynd.png

... in 0..1 range

---

Also visualize via:
    https://graphtoy.com/

*/

precision mediump float;

uniform vec2 u_resolution;

#define PI 3.14159

const vec3 RED = vec3(1.0, 0.0, 0.0);
const vec3 YELLOW = vec3(1.0, 1.0, 0.0);
const vec4 BLACK = vec4(0.0, 0.0, 0.0, 1.0);
const float STROKE_WIDTH = 5.0 / 1000.0;

void main() {
    vec2 uv_a = (gl_FragCoord.xy / u_resolution.xy);
    vec2 uv_b = uv_a * 2.0 - 1.0;

    // [0, 1] coords
    float a1 = cos(uv_a.x);
    float a2 = cos(PI * uv_a.x);

    // [-1, 1] coords
    float b1 = cos(uv_b.x);
    float b2 = cos(PI * uv_b.x);

    vec4 color;
    if (abs(uv_a.y - a1) < STROKE_WIDTH / 2.0)
        color = vec4(RED, 1.0);
    else if (abs(uv_a.y - a2) < STROKE_WIDTH / 2.0)
        color = vec4(YELLOW, 1.0);
    else if (abs(uv_b.y - b1) < STROKE_WIDTH)
        color = vec4(RED, 0.1);
    else if (abs(uv_b.y - b2) < STROKE_WIDTH)
        color = vec4(YELLOW, 0.1);
    else
        color = BLACK;

    gl_FragColor = color;
}
