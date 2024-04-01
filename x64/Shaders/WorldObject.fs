#version 430 core

in vec3 FragPos;
in vec3 Normal;
in vec4 Color;
in vec2 TexCoord;

uniform sampler2D texture1;
// Uniform for mode selection
// 0 = Color, 1 = Texture, 2 = Both
uniform int mode;
uniform float colorAlpha;
uniform float opacity;


// Outputs for the G-Buffer (position, normal, albedo, etc.)
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 gPosition;


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
        vec4 mixedColor = mix(textureColor, Color, Color.w) * opacity; // Mix based on colorAlpha
        FragColor = mixedColor;
    }

    gPosition = vec4(FragPos, 1.0);
    
}
