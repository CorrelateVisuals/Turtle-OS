precision mediump float;

varying vec2 TexCoord;
uniform sampler2D tex;

void main() {
    vec4 color = texture2D(tex, TexCoord);
    if (color.a == 0.0) {
        // Background: draw grid on dark grey
        vec4 bg = vec4(0.2, 0.2, 0.2, 1.0);
        if (mod(gl_FragCoord.x, 50.0) < 5.0 || mod(gl_FragCoord.y, 50.0) < 5.0) {
            bg = vec4(0.3, 0.3, 0.3, 1.0);
        }
        gl_FragColor = bg;
    } else {
        gl_FragColor = color;
    }
}
