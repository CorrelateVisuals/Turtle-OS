attribute vec3 pos;
attribute vec2 uv;

uniform float scale;

varying vec2 TexCoord;

void main() {
    vec2 scaled = pos.xy * scale;
    gl_Position = vec4(scaled, pos.z, 1.0);
    TexCoord = uv;
}
