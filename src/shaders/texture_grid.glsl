precision mediump float;

varying vec2 TexCoord;
uniform sampler2D tex;

void main() {
    // Draw grid on black with white lines
    vec4 bg = vec4(0.0, 0.0, 0.0, 1.0);
    if (mod(gl_FragCoord.x, 20.0) < 10.0 || mod(gl_FragCoord.y, 20.0) < 10.0) {
        bg = vec4(1.0, 1.0, 1.0, 1.0);
    }
    gl_FragColor = bg;
}
