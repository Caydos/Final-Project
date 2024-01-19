#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 aMatrice;

out vec4 Color;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform vec4 color;

void main()
{
    gl_Position = projection * model * vec4(aPos, 1.0f);
	Color = color;
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}
