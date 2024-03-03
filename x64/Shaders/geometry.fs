#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
// in vec4 Color;
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

uniform sampler2D texture1;
// Uniform for mode selection
// 0 = Color, 1 = Texture, 2 = Both
uniform int mode;
uniform float colorAlpha;
uniform float opacity;
uniform bool lightDependent;
uniform bool considerLightning;

void main()
{    
    gPosition = FragPos;
    gNormal = normalize(Normal);

    gAlbedoSpec.rgb = texture(texture1, TexCoords).rgb;
    gAlbedoSpec.a = 1.0;
}
