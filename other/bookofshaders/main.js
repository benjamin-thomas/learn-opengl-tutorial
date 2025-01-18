const vertexShaderSource = /*glsl*/`
    attribute vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
`;

const fragmentShaderSource = /* glsl */`
    precision mediump float;
    void main() {
        gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
`;

const fragmentShader2Source = /* glsl */`
    precision mediump float;
    void main() {
        gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    }
`;

/**
 *
 * @param {WebGLRenderingContext} gl
 * @param {GLenum} type
 * @param {string} source
 */
function createShader(gl, type, source) {
    const shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);

    const success = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
    if (!success) {
        gl.deleteShader(shader);
        throw new Error('Shader compilation failed', gl.getShaderInfoLog(shader));
    }

    return shader;
}

/**
 *
 * @param {WebGLRenderingContext} gl
 * @param {WebGLShader} vertexShader
 * @param {WebGLShader} fragmentShader
 */
function createProgram(gl, vertexShader, fragmentShader) {
    const program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);

    const success = gl.getProgramParameter(program, gl.LINK_STATUS);
    if (!success) {
        gl.deleteProgram(program);
        throw new Error('Program linking failed', gl.getProgramInfoLog(program));
    }

    return program;
}

function main() {
    const canvas = document.querySelector('#gl-canvas');

    /**
     * @type {WebGLRenderingContext}
     */
    const gl = canvas.getContext('webgl');

    if (!gl) {
        canvas.outerHTML = "Sorry, webGL is not supported in this browser.";
        return;
    }

    // Create shaders
    const vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexShaderSource);
    const fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentShaderSource);
    const fragmentShader2 = createShader(gl, gl.FRAGMENT_SHADER, fragmentShader2Source);

    // Create shader programs
    const program = createProgram(gl, vertexShader, fragmentShader);
    const program2 = createProgram(gl, vertexShader, fragmentShader2);

    // Data points
    const triangles = [
        new Float32Array([
            -0.45, 0.5, 0.0,   // t
            -0.8, -0.5, 0.0,   // bl
            -0.1, -0.5, 0.0    // br
        ]),
        new Float32Array([
            0.45, 0.5, 0.0,    // t
            0.8, -0.5, 0.0,    // br
            0.1, -0.5, 0.0     // bl
        ])
    ];

    const buffers = triangles.map(() => gl.createBuffer());

    // Get attribute locations
    const posAttributeLocation = gl.getAttribLocation(program, 'aPos');

    const render = () => {
        console.log("Rendering...");

        gl.clearColor(0.2, 0.3, 0.3, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);

        for (let i = 0; i < triangles.length; i++) {
            gl.bindBuffer(gl.ARRAY_BUFFER, buffers[i]);
            gl.bufferData(gl.ARRAY_BUFFER, triangles[i], gl.STATIC_DRAW);
            gl.enableVertexAttribArray(posAttributeLocation);
            gl.vertexAttribPointer(posAttributeLocation, 3, gl.FLOAT, false, 0, 0);

            const currentProgram = i === 0 ? program : program2;
            gl.useProgram(currentProgram);

            // Draw
            gl.drawArrays(gl.TRIANGLES, 0, 3);
        };
        requestAnimationFrame(render);
    };

    render();
}

main();