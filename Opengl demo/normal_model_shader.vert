#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec3 tFragPos;
out vec2 TexCoords;
out PointLight tPointLight;
out vec3 tViewPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 ViewPos;

uniform PointLight pointLight;

void main()
{
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,  0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));

    mat3 TBN = transpose(mat3 (T, B, N));
    tViewPos = TBN * ViewPos;
    vec3 lightPos = TBN * pointLight.position;
    tPointLight = pointLight;
    tPointLight.position = lightPos;
    tFragPos = TBN * FragPos;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}