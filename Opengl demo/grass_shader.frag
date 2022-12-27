#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D grassTexture;

void main()
{
    vec4 texColor = texture(grassTexture, TexCoords);
    FragColor =  texColor;// set all 4 vector values to 1.0
}