#version 330 core
out vec4 FragColor;
in vec3 outColor;
in vec2 texCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixValue;

void main()
{
FragColor= mix(texture(texture1, texCoords), texture(texture2,vec2(-texCoords.x,texCoords.y)),mixValue) * vec4(outColor, 0.25f);
}