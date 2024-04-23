#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 aMatrice;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool instanceUsage;

void main()
{
    mat4 effectiveModel = instanceUsage ? aMatrice : model;
    mat3 normalTransformation = mat3(transpose(inverse(effectiveModel)));
    vec3 tangent;
    vec3 bitangent;
    // Quad automation (only for quads)
    if (aNormal == vec3(0.0, 0.0, 1.0))
    {// Front face
        tangent = vec3(1.0, 0.0, 0.0);
        bitangent = vec3(0.0, 1.0, 0.0);
    } else if (aNormal == vec3(0.0, 0.0, -1.0))
    {// Back face
        tangent = vec3(-1.0, 0.0, 0.0);
        bitangent = vec3(0.0, 1.0, 0.0);
    } else if (aNormal == vec3(1.0, 0.0, 0.0))
    {// Right face
        tangent = vec3(0.0, 0.0, -1.0);
        bitangent = vec3(0.0, 1.0, 0.0);
    } else if (aNormal == vec3(-1.0, 0.0, 0.0))
    {// Left face
        tangent = vec3(0.0, 0.0, 1.0);
        bitangent = vec3(0.0, 1.0, 0.0);
    } else if (aNormal == vec3(0.0, 1.0, 0.0))
    {// Top face
        tangent = vec3(1.0, 0.0, 0.0);
        bitangent = vec3(0.0, 0.0, -1.0);
    } else if (aNormal == vec3(0.0, -1.0, 0.0))
    {// Bottom face
        tangent = vec3(1.0, 0.0, 0.0);
        bitangent = vec3(0.0, 0.0, 1.0);
    }
    tangent = normalize(normalTransformation * tangent);
    bitangent = normalize(normalTransformation * bitangent);
    TBN = mat3(tangent, bitangent, normalize(normalTransformation * aNormal));

    
    // {//Better normal interpolation (A bit more calculations ofc) - Gram-Schmidt process
    //     tangent = vec3(vec4(tangent,0.0) * effectiveModel);
    //     bitangent = vec3(vec4(bitangent,0.0) * effectiveModel);

    //     tangent = normalize(tangent - dot(tangent, aNormal) * aNormal);
    //     bitangent = cross(aNormal, tangent);
    // }
    // Create TBN matrix
    // TBN = mat3(tangent, bitangent, aNormal);

    WorldPos = vec3(effectiveModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(effectiveModel))) * aNormal;
    gl_Position = projection * view * effectiveModel * vec4(aPos, 1.0);
    TexCoords = vec2(aTexCoords.x, 1.0 - aTexCoords.y);

}
