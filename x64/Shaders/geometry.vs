#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 aMatrice;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool instanceUsage;

void main()
{
    mat4 effectiveModel = instanceUsage ? aMatrice : model;

    WorldPos = vec3(effectiveModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(effectiveModel))) * aNormal;
    gl_Position = projection * view * effectiveModel * vec4(aPos, 1.0);
    TexCoords = vec2(aTexCoords.x, 1.0 - aTexCoords.y);

}
