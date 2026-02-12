precision mediump float;

varying vec2 TexCoord;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float mix_factor;

void main() {
    vec4 base = texture2D(tex0, TexCoord);
    vec4 overlay = texture2D(tex1, TexCoord);
    gl_FragColor = mix(base, overlay, mix_factor);
}
