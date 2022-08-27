#version 430
in vec2 textureCoord;
out vec4 frag_color;

uniform sampler2D tex;
uniform float alpha;

void main() {   

   frag_color = texture(tex, textureCoord);
   frag_color.a = frag_color.a * alpha;
   if (frag_color.a == 0.0)
   {
	discard;
   }
}