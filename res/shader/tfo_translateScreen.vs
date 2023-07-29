attribute vec4 pos;

uniform mat4 V;
uniform mat4 P;

void main()
{
	gl_PointSize=1000.0;
    gl_Position=P*V*pos;
}