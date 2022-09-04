#version 430
layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coords;
out vec2 textureCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform float layer;

void main() {
   textureCoord = texture_coords;
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, layer, 1.0);
}