#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gEffects;

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2
struct Light
{
    bool activated;
    int type;

    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_LIGHTS 40
uniform Light lights[MAX_LIGHTS];
uniform int lightCount;
uniform bool considerLightning;

void main()
{    
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    // float Specular = texture(gEffects, TexCoords).a;
    float Specular = 1.0;
       
    // if (considerLightning)
    // {
    //     vec3 viewDir = normalize(FragPos);

    //     vec3 result;
    //     for (int i = 0; i < lightCount; i++)
    //     {
    //         if (lights[i].activated)
    //         {
    //             if (lights[i].type == DIRECTIONAL)
    //             {
    //                 // result += CalcDirLight(lights[i]);
    //             }
    //             else if (lights[i].type == POINT)
    //             {
    //                 // result += CalcPointLight(lights[i]);
    //             }
    //             else if (lights[i].type == SPOT)
    //             {
    //                 // result += CalcSpotLight(lights[i]);
    //             }
    //         }
    //     }
        
    //     FragColor = vec4(result, 1.0);
    //     return;
    // }

    FragColor = vec4(Albedo.x, Albedo.y, Albedo.z, 1.0);
}

