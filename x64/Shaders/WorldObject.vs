#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 aMatrice;

out vec3 FragPos;
out vec3 Normal;
out vec4 Color;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool instanceUsage;
uniform vec4 color;

void main()
{
    if (!instanceUsage)
    {
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal;
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
    }
    else
    {
        FragPos = vec3(aMatrice * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(aMatrice))) * aNormal;
        gl_Position = projection * view * aMatrice * vec4(aPos, 1.0f);
    }

	Color = color;
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}