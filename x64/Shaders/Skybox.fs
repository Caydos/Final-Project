#version 430 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gEffects;



in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    gAlbedoSpec = texture(skybox, TexCoords);
}
