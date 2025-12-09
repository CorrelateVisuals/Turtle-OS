attribute vec2 pos;
attribute vec2 uv;

uniform float scale;

varying vec2 TexCoord;

void main() {
    vec2 scaled = pos * scale;
    gl_Position = vec4(scaled, 0.0, 1.0);
    TexCoord = uv;
}
