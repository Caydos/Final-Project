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
uniform vec4 clearColor;

vec3 CalcSpotLight(Light _light, vec3 _normal, vec3 _worldPos, vec3 _viewDir, vec3 _albedo, float _specular, float _shininess)
{
    vec3 lightDir = normalize(_light.position - _worldPos);
    // diffuse shading
    float diff = max(dot(_normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0), _shininess);
    // attenuation
    float distance = length(_light.position - _worldPos);
    float attenuation = 1.0 / (_light.constant + _light.linear * distance + _light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-_light.direction)); 
    float epsilon = _light.cutOff - _light.outerCutOff;
    float intensity = clamp((theta - _light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = _light.ambient * _albedo;
    vec3 diffuse = _light.diffuse * diff * _albedo;
    vec3 specular = _light.specular * spec * vec3(_specular,_specular,_specular);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 WorldPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Shininess = texture(gEffects, TexCoords).r;
    float Specular = texture(gEffects, TexCoords).a;
    if((Normal.x == clearColor.x) && (Normal.y == clearColor.y) && (Normal.z == clearColor.z))
    {
        FragColor = vec4(Albedo.x, Albedo.y, Albedo.z, 1.0);
        return;
    }
   
    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - WorldPos);

    vec3 result = CalcSpotLight(lights[0], norm, WorldPos, viewDir, Albedo, Specular, Shininess*100);
    FragColor = vec4(result.x, result.y, result.z, 1.0);
}

