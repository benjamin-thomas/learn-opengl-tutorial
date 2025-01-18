precision mediump float;

uniform vec2 u_resolution;

void main() {
    vec2 grid = vec2(u_resolution.xy / 10.0);

    vec2 uv = gl_FragCoord.xy;

    vec4 color = (int(mod(uv.x, grid.x)) == 0 || int(mod(uv.y, grid.y)) == 0)
                     ? vec4(0.0, 0.0, 0.0, 0.0)
                     : vec4(1.0, 1.0, 1.0, 1.0);

    gl_FragColor = color;
}