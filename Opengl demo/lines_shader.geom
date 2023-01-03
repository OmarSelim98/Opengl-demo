#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT{
	vec3 color;
} gs_in[];

out vec3 fsColor;

void main(){
	fsColor = gs_in[0].color;

	gl_Position = gl_in[0].gl_Position + vec4(-0.2, -0.2, 0.0, 0.0); // bottom-left
	EmitVertex();
	fsColor = gs_in[0].color - 0.5;

	gl_Position = gl_in[0].gl_Position + vec4(0.2, -0.2, 0.0, 0.0); // bottom-right
	EmitVertex();
	fsColor = gs_in[0].color + 1.0;

	gl_Position = gl_in[0].gl_Position + vec4(-0.2, 0.2, 0.0, 0.0); // top-left
	EmitVertex();
	fsColor = gs_in[0].color;

	gl_Position = gl_in[0].gl_Position + vec4(0.2, 0.2, 0.0, 0.0); // top-right
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.4,0.0 ,0.0); // top-center
	EmitVertex();

	EndPrimitive();
}