#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices=6) out;

const float MAGNITUDE = 0.4;

out VS_OUT {
    vec3 normal;
} gs_in[];

uniform mat4 p;

void GenerateLine(int index){
    gl_Position = p * gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = p * (gl_in[index].gl_Position + vec4(gs_in[index].normal,0.0) * MAGNITUDE);
    EmitVertex();

    EndPrimitive();
}

void main(){
GenerateLine(0);
GenerateLine(1);
GenerateLine(2);
}