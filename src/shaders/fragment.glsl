precision mediump float;

varying vec2 TexCoord;
uniform sampler2D tex;

void main() {
    vec4 color = texture2D(tex, TexCoord);
    float grid = 0.0;
    if (mod(gl_FragCoord.x, 50.0) < 2.0) grid = 1.0;
    if (mod(gl_FragCoord.y, 50.0) < 2.0) grid = 1.0;
    color = mix(color, vec4(0.3, 0.3, 0.3, 1.0), grid * 0.5);
    gl_FragColor = color;
}
