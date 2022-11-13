#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
out VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
} vs_out;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
    vs_out.TexCoords = aTexCoords;
    vec4 glPos = projection * view * vec4(vs_out.FragPos, 1.0f);
    gl_Position = glPos;
}