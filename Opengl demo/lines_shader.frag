#version 330 core
out vec4 FragColor;
in  vec3 fsColor;

void main(){
	FragColor = vec4(fsColor,1.0);
}