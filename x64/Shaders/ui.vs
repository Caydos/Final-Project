#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 Color;
out vec2 TexCoord;

uniform mat4 transform;
uniform vec4 color;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	Color = color;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}