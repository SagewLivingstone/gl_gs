#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 _vertexColor;

out vec3 vertexColor;

void main()
{
    vertexColor = _vertexColor;
    gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0);
}
