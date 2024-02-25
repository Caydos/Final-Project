#version 330 core
out vec4 FragColor;

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

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


in vec3 FragPos;
in vec3 Normal;
in vec4 Color;
in vec2 TexCoord;


#define MAX_LIGHTS 40


uniform vec3 viewPos;
uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform Material material;

uniform sampler2D texture1;
// Uniform for mode selection
// 0 = Color, 1 = Texture, 2 = Both
uniform int mode;
uniform float colorAlpha;
uniform float opacity;
uniform bool lightDependent;
uniform bool considerLightning;


vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

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

    if (considerLightning && lightDependent)
    {
        vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(/*viewPos -*/ FragPos);/* View pos if we want it to be tracked down by the camera position*/

        // vec3 result = vec3(FragColor.x, FragColor.y, FragColor.z);
        vec3 result;
        for (int i = 0; i < lightCount; i++)
        {
            if (lights[i].activated)
            {
                if (lights[i].type == DIRECTIONAL)
                {
                    result += CalcDirLight(lights[i], norm, viewDir);
                }
                else if (lights[i].type == POINT)
                {
                    result += CalcPointLight(lights[i], norm, FragPos, viewDir);
                }
                else if (lights[i].type == SPOT)
                {
                    result += CalcSpotLight(lights[i], norm, FragPos, viewDir);
                }
            }
        }
        
        FragColor = vec4(result, 1.0);
    }
}



vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}