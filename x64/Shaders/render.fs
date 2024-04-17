#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gEffects;
uniform sampler2D gLighting;


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

uniform vec3 viewPos;
uniform vec4 clearColor;

vec3 CalcDirLight(Light _light, vec3 _normal, vec3 _viewDir, vec3 _albedo, float _specular, float _shininess)
{
    vec3 lightDir = normalize(-_light.direction);
    // diffuse shading
    float diff = max(dot(_normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0), _shininess);
    // combine results
    vec3 ambient = _light.ambient * _albedo;
    vec3 diffuse = _light.diffuse * diff * _albedo;
    vec3 specular = _light.specular * spec * vec3(_specular,_specular,_specular);
    return (ambient + diffuse + specular);
}


void main()
{
    vec3 WorldPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    vec3 Lighting = texture(gLighting, TexCoords).rgb;
    // float Shininess = texture(gEffects, TexCoords).r;
    // float Specular = texture(gEffects, TexCoords).a;
    if((Normal.x == clearColor.x) && (Normal.y == clearColor.y) && (Normal.z == clearColor.z))
    {// Need to be a GL_RGBA32F texture
        FragColor = vec4(Albedo, 1.0);
        return;
    }

    // vec3 norm = normalize(Normal);
    // vec3 viewDir = normalize(viewPos - WorldPos);
    // vec3 result;

    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    if((Lighting.x == clearColor.x) && (Lighting.y == clearColor.y) && (Lighting.z == clearColor.z))
    {// Need to be a GL_RGBA32F texture
        FragColor = vec4(0.0,0.0,0.0,1.0);
        return;
    }
    FragColor = vec4(Lighting, 1.0);
}

