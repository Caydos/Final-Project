#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoord;

uniform sampler2D texture1;
// Uniform for mode selection
// 0 = Color, 1 = Texture, 2 = Both
uniform int mode;
uniform float opacity;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

void main()
{
	vec4 color = Color; // Color with full alpha
    vec4 textureColor = texture(texture1, TexCoord); // Texture color

    if (mode == 0)
    {// Apply only color
        FragColor = Color * opacity;
    }
    else if (mode == 1)
    {// Apply only texture
        FragColor = texture(texture1, TexCoord) * opacity;
    }
    else if (mode == 2)
    {// Apply both texture and color
        vec4 mixedColor = mix(textureColor, color, 1.0f) * opacity; // Mix based on colorAlpha
        FragColor = mixedColor;
    }
}