#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out VS_OUT{
	vec3 color;
} vs_out;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

void main(){
	gl_Position = p * v * m * vec4(aPos.x, aPos.y, 0.0, 1.0);
	vs_out.color = aColor;
}