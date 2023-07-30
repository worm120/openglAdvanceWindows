#version 400
layout(triangles,equal_spacing,ccw)in;

void main()
{
    //(x,y,z) -> top(0,1,0) lb(0,0,1) rb(1,0,0)
	float u=gl_TessCoord.x;
	float v=gl_TessCoord.y;
	float w=gl_TessCoord.z;
	if(w==1.0)
	{
		gl_Position=gl_in[0].gl_Position;
	}
	else if(u==1.0)
	{
		gl_Position=gl_in[2].gl_Position;
	}
	else
	{
		gl_Position=gl_in[1].gl_Position;
	}
}