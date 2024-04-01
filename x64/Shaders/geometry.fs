#version 430 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gEffects;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;


uniform sampler2D textureX;
uniform sampler2D effectsTexture;

uniform float shininess;

void main()
{    
    gPosition = WorldPos;
    gNormal = normalize(Normal);

    gAlbedoSpec.rgb = texture(textureX, TexCoords).rgb;
    gAlbedoSpec.a = 1.0;
    // gEffects.r = shininess;
    gEffects.r = shininess / 100;
    gEffects.a = texture(effectsTexture, TexCoords).a;
}
