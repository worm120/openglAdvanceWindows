#version 330 core
layout(points)in;
layout(triangle_strip, max_vertices = 4)out;

out vec2 V_Texcoord;
uniform mat4 P;
void main()
{
	vec4 pos=gl_in[0].gl_Position;
	float spriteSize = 0.4;

	gl_Position = P*(pos + vec4(-spriteSize,-spriteSize,0.0,1.0));
	V_Texcoord = vec2(0.0,0.0);
	EmitVertex();

	gl_Position = P*(pos + vec4(spriteSize, -spriteSize, 0.0, 1.0));
	V_Texcoord = vec2(1.0, 0.0);
	EmitVertex();

	gl_Position = P*(pos + vec4(-spriteSize, spriteSize, 0.0, 1.0));
	V_Texcoord = vec2(0.0, 1.0);
	EmitVertex();

	gl_Position = P*(pos + vec4(spriteSize, spriteSize, 0.0, 1.0));
	V_Texcoord = vec2(1.0, 1.0);
	EmitVertex();

	EndPrimitive();
}