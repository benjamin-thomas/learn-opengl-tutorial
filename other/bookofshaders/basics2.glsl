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

const vec3 RED = vec3(1.0, 0.0, 0.0);
const vec3 YELLOW = vec3(1.0, 1.0, 0.0);
const vec4 BLACK = vec4(0.0, 0.0, 0.0, 1.0);
const float STROKE_WIDTH = 5.0 / 1000.0;

void main() {
    // Transform to -1 to 1 range
    // Formula: value * 2 - 1 transforms from [0,1] to [-1,1]
    vec2 uv = (gl_FragCoord.xy / u_resolution.xy) * 2.0 - 1.0;

    float d1 = abs(uv.x);                  // diag
    float d2 = 1.0 - abs(uv.x);            // diag flipped (on the x axis)
    float c1 = pow(abs(uv.x), 0.3);        // curved
    float c2 = 1.0 - pow(abs(uv.x), 0.3);  // curved flipped (on the x axis)

    vec4 color;
    if (abs(uv.y - d1) < STROKE_WIDTH)
        color = vec4(RED, 1.0);
    else if (abs(uv.y - d2) < STROKE_WIDTH)
        color = vec4(RED, 0.1);
    else if (abs(uv.y - c1) < STROKE_WIDTH)
        color = vec4(YELLOW, 1.0);
    else if (abs(uv.y - c2) < STROKE_WIDTH)
        color = vec4(YELLOW, 0.1);
    else
        color = BLACK;

    gl_FragColor = color;
}
