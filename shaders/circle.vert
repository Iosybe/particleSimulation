#version 330 core

layout(location = 0) in vec2 vertex;

uniform vec2 translation;
uniform vec2 scale;

void main() {
    gl_Position.x = vertex.x * scale.x + translation.x;
    gl_Position.y = vertex.y * scale.y + translation.y;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
}