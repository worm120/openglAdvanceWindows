#version 400
layout(quads,equal_spacing,ccw)in;

void main()
{
    //(x,y,z) -> top(0,1,0) lb(0,0,1) rb(1,0,0)
	float u=gl_TessCoord.x;
	float v=gl_TessCoord.y;
	float w=gl_TessCoord.z;

	gl_Position=gl_in[0].gl_Position*(1-u)*(1-v)+gl_in[1].gl_Position*(1-v)*u
		+gl_in[3].gl_Position*v*(1-u)+gl_in[2].gl_Position*u*v;
}