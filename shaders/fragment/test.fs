#version 330 core

in vec3 vertexColor;

uniform float greenValue;

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.0, greenValue, 0.0, 1.0);
}