#version 430 core
layout (location = 0) out vec4 gLightingSpec;


uniform sampler2D inPosition;
uniform sampler2D inNormal;
uniform sampler2D inAlbedoSpec;
uniform sampler2D inEffects;
// uniform sampler2D inLighting;

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
in float outActivation;

// calculates the color when using a spot light.
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
    // float attenuation = 1.0 / (outConstant + outLinear * distance + outQuadratic * (distance * distance));    
    float attenuation = 1.0 / (outConstant * distance * distance* distance * distance* distance * distance);
    attenuation = min(attenuation, 1.0);

    // spotlight intensity
    float theta = dot(lightDir, normalize(-outDirection)); 
    float epsilon = outCutOff - outOuterCutOff;
    float intensity = clamp((theta - outOuterCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = outAmbient * _albedo;
    vec3 diffuse = outDiffuse * diff * _albedo;
    // vec3 specular = outSpecular * spec * vec3(_specular,_specular,_specular);
    vec3 specular = vec3(0.0);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

uniform vec3 viewPos;
uniform vec4 clearColor;
uniform vec2 screenSize;
void main() 
{
    if (outActivation == 0)
    {
        gLightingSpec = vec4(1.0,0.0,0.0,1.0);
        discard;
        // return;
    }
    vec2 ndc = gl_FragCoord.xy / screenSize;
    vec3 WorldPos = texture(inPosition, ndc).rgb;
    vec3 Normal = texture(inNormal, ndc).rgb;
    vec3 Albedo = texture(inAlbedoSpec, ndc).rgb;
    // float Shininess = texture(inEffects, ndc).r;
    float Shininess = 0.0;
    // float Specular = texture(inEffects, ndc).a;
    float Specular = 0.0;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - WorldPos);

    // if((Normal.x == clearColor.x) && (Normal.y == clearColor.y) && (Normal.z == clearColor.z))
    // {
    //     FragColor = vec4(Albedo.x, Albedo.y, Albedo.z, 1.0);
    //     return;
    // }

    // gLightingSpec = vec4(CalcSpotLight(norm, WorldPos, viewDir, Albedo, Specular, Shininess*100), 1.0);
    gLightingSpec = vec4(Albedo, 1.0);
}