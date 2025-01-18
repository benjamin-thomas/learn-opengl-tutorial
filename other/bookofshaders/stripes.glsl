#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

float angle = -0.5;
float speed = 0.1;
float divisions = 8.0;
float stripe_bias = 2.5;

vec4 color_gap = vec4(0.25);
vec4 color_stripe = vec4(1.0, 0.75, 0.0, 1.0);

void main() {
    vec2 uv = (gl_FragCoord.xy / u_resolution.xy);

    float w = cos(angle) * uv.x + sin(angle) * uv.y - speed * u_time;
    vec4 color;
    if (int(floor(mod(w * divisions, stripe_bias))) == 0) {
        color = color_gap;
    } else {
        color = color_stripe;
    }

    gl_FragColor = color;
}
