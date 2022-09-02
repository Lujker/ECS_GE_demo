#version 430
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coords;

out vec2 Texture_coord;
out vec3 Normal;
out vec3 Fragment_pos;  

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0);
   Texture_coord = texture_coords;
   Normal = normal;
   Fragment_pos = vec3(modelMatrix * vec4(vertex_position, 1.0f));
}