#version 430 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 direction;
layout (location = 3) in float cutOff;
layout (location = 4) in float outerCutOff;
layout (location = 5) in vec3 ambient;
layout (location = 6) in vec3 diffuse;
layout (location = 7) in vec3 specular;
layout (location = 8) in float constant;
layout (location = 9) in float linear;
layout (location = 10) in float quadratic;
layout (location = 11) in float activation;
layout (location = 12) in mat4 modelMatrix;

uniform mat4 view;
uniform mat4 projection;




out vec3 outPosition;
out vec3 outDirection;
out float outCutOff;
out float outOuterCutOff;
out vec3 outAmbient;
out vec3 outDiffuse;
out vec3 outSpecular;
out float outConstant;
out float outLinear;
out float outQuadratic;
out float outActivation;


void main()
{
    outPosition = position;
    outDirection = direction;
    outCutOff = cutOff;
    outOuterCutOff = outerCutOff;
    outAmbient = ambient;
    outDiffuse = diffuse;
    outSpecular = specular;
    outConstant = constant;
    outLinear = linear;
    outQuadratic = quadratic;
    outActivation = activation;

    if (activation == 0)
    {
        gl_Position = vec4(-9999.0, -9999.0, -9999.0, 1.0);
        return;
    }
    vec4 worldPosition = modelMatrix * vec4(vertexPos, 1.0);
    vec4 viewPosition = view * worldPosition;
    gl_Position = projection * viewPosition;
}