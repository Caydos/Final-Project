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
uniform vec3 viewPos;
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float outerCutOff = 0.96363;
    float cutOff = 0.976296;
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = vec3(0.0f, 0.0f, 0.0f) * vec3(0.5,0.5,0.5);
    vec3 diffuse = vec3(1.0f, 1.0f, 1.0f) * diff * vec3(0.5,0.5,0.5);
    vec3 specular = vec3(1.0f, 1.0f, 1.0f) * spec * vec3(1.0,1.0,1.0);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    // float Specular = texture(gEffects, TexCoords).a;
    float Specular = 1.0;

    // vec3 norm = normalize(Normal);
    // vec3 viewDir = normalize(viewPos - FragPos);

    // vec3 result = CalcSpotLight(lights[0], norm, FragPos, viewDir);
    vec3 color = normalize(Normal) * 0.5 + 0.5;
    FragColor = vec4(color.x, color.y, color.z, 1.0);
    // FragColor = vec4(Albedo.x, Albedo.y, Albedo.z, 1.0);
}

