#version 330 core
out vec4 color;

uniform vec4 LightColor;

void main()
{
    color = LightColor; // Устанавливает все 4 компоненты вектора равными 1.0f
}