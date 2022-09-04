#version 330 core
in vec2 textureCoord;
out vec4 color;

uniform sampler2D tex;
uniform vec3 textColor;
uniform float alpha;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, textureCoord).r * alpha);
    color = vec4(textColor, 1.0) * sampled;
}