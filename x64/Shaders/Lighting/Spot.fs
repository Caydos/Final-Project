#version 430 core

out vec4 FragColor;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gEffects;

in vec3 outPosition;
in vec3 outDirection;
in float outCutOff;
in float outOuterCutOff;
in vec3 outAmbient;
in vec3 outDiffuse;
in vec3 outSpecular;
in float outConstant;
in float outLinear;
in float outQuadratic;



vec3 CalcSpotLight(vec3 _normal, vec3 _worldPos, vec3 _viewDir, vec3 _albedo, float _specular, float _shininess)
{
    vec3 lightDir = normalize(outPosition - _worldPos);
    // diffuse shading
    float diff = max(dot(_normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0), _shininess);
    // attenuation
    float distance = length(outPosition - _worldPos);
    float attenuation = 1.0 / (outConstant + outLinear * distance + outQuadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-outDirection)); 
    float epsilon = outCutOff - outOuterCutOff;
    float intensity = clamp((theta - outOuterCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = outAmbient * _albedo;
    vec3 diffuse = outDiffuse * diff * _albedo;
    vec3 specular = outSpecular * spec * vec3(_specular,_specular,_specular);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

uniform vec3 viewPos;
uniform vec4 clearColor;

void main() 
{
    vec2 TexCoords = vec2(0.0,0.0);

    vec3 WorldPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Shininess = texture(gEffects, TexCoords).r;
    float Specular = texture(gEffects, TexCoords).a;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - WorldPos);

    if((Normal.x == clearColor.x) && (Normal.y == clearColor.y) && (Normal.z == clearColor.z))
    {
        FragColor = vec4(Albedo.x, Albedo.y, Albedo.z, 1.0);
        return;
    }

    FragColor = vec4(CalcSpotLight(norm, WorldPos, viewDir, Albedo, Specular, Shininess*100), 1.0);
}