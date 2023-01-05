#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 Color;

void main(){
	Color = aColor;
	vec2 newPos = aPos * (gl_InstanceID+1/50.0);
	gl_Position = vec4(newPos + aOffset, 0.0, 1.0);
}