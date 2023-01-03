#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D mirrorTexture;
uniform sampler2D mirrorTexture1;

void main(){
    /** NORMAL */
    FragColor =  mix(texture(mirrorTexture, TexCoords), texture(mirrorTexture1, TexCoords), 0.2);
}