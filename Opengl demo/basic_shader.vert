#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 outColor;
out vec2 texCoords;

void main()
{
   gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   outColor = aColor;
   texCoords = aTexCoords;
};