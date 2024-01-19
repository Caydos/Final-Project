#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoord;

// Texture sampler
uniform sampler2D texture1;

// Uniform for mode selection
// 0 = Color, 1 = Texture, 2 = Both
uniform int mode;
uniform float opacity;

void main()
{
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
        vec4 mixedColor = mix(textureColor, Color, 1.0f) * opacity; // Mix based on colorAlpha
        FragColor = mixedColor;
    }
}
