#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gEffects;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;


uniform sampler2D textureX;
uniform sampler2D effectsTexture;

// Uniform for mode selection
// 0 = Color, 1 = Texture, 2 = Both
uniform int mode;
uniform float colorAlpha;
uniform float opacity;

void main()
{    
    gPosition = FragPos;
    gNormal = normalize(Normal);

    gAlbedoSpec.rgb = texture(textureX, TexCoords).rgb;
    gAlbedoSpec.a = 1.0;
    gEffects.a = texture(effectsTexture, TexCoords).a;
}
