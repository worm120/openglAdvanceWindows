#version 430

struct Vertex
{
	vec4 pos;
	vec4 texcoord;
	vec4 normal;
};

layout(std140,binding=0)buffer Mesh{
	Vertex vertexes[];
}mesh;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

varying vec2 V_Texcoord;

void main()
{
	V_Texcoord=mesh.vertexes[gl_VertexID].texcoord.xy;

    gl_Position=P*V*M*vec4(mesh.vertexes[gl_VertexID].pos.xyz,1.0);
}