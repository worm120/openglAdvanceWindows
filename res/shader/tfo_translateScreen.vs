attribute vec4 pos;
attribute vec4 mess;

uniform mat4 V;
uniform mat4 P;


varying vec4 V_Color;

void main()
{
	gl_PointSize=1000.0;
	V_Color.a=pow(sin(3.14*(mess.x/10.0)),2.0);
    gl_Position=P*V*pos;
}