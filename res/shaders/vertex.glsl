# version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertex_color;
out vec4 color;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
void main()
{
    color = vertex_color;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}