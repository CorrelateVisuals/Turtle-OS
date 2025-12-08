attribute vec2 pos;
uniform float scale;

void main() {
    vec2 scaled = pos * scale;
    gl_Position = vec4(scaled, 0.0, 1.0);
}
