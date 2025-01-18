/*

Tweaked from: https://glslsandbox.com/e#109698.0

*/

#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float u_time;
uniform vec2 u_mouse;
uniform vec2 u_resolution;

vec2 axel = vec2(1.0);
const float count = 138.0;
float brght = 0.01;  //1.0/count;
float dist = 1.8;
float radius = 0.01;

float l = 1.;
float w = 1.;

void main(void) {
    axel = u_mouse;
    axel = vec2(0.01, -0.04);
    // axel += (u_mouse - axel) / 5.0;

    vec3 Color = vec3(0.26, 0.15, 0.15);
    float col = -0.5;
    vec2 centr = 2.0 * (gl_FragCoord.xy * 2.0 - u_resolution) /
                 min(u_resolution.x, u_resolution.y);

    for (float i = 0.0; i < count; i++) {
        float si = sin(u_time * 3.0 + i * dist * 4.0 * axel.x * 4.0) * l * 0.4;
        float co = cos(u_time * 1.4 + i * dist * axel.y) * w * 0.4;

        col += brght / abs(length(centr + vec2(si, co)) - radius);
    }

    gl_FragColor = vec4(vec3(Color * col), 1.0);
}