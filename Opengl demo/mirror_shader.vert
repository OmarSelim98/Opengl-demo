#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
//out vec3 Normal;
//out vec3 FragPos;
out vec2 TexCoords;
//out mat4 ScreenSpaceMat;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
    //FragPos = vec3(model * vec4(aPos,1.0));
    //Normal = mat3(transpose(inverse(model)))* aNormal;
    //ScreenSpaceMat = view;
    TexCoords = aTexCoords;
} 