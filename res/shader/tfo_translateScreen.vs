attribute vec4 pos;
attribute vec4 mess;

uniform mat4 V;

varying vec4 vs_Color;
varying vec4 vs_Mess;

void main()
{
	vs_Color.a=pow(sin(3.14*(mess.x/5.0)),2.0);
	vs_Mess=mess;
    gl_Position=V*pos;
}