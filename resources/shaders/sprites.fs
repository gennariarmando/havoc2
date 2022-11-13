#version 330 core
out vec4 FragColor;
in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
} fs_in;
uniform sampler2D sprite;
uniform vec4 color;
void main() {
    vec4 texColor = texture(sprite, fs_in.TexCoords);
    if (texColor.a < 0.5f)
        discard;
    FragColor = texColor;
    FragColor *= color;
}
