#version 430 core

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

in vec3 outPosition;
in vec3 outDirection;
in float outCutOff;
in float outOuterCutOff;
in vec3 outAmbient;
in vec3 outDiffuse;
in vec3 outSpecular;
in vec3 outConstant;
in vec3 outLinear;
in vec3 outQuadratic;

void main() 
{
    
}