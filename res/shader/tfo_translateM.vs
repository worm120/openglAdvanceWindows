attribute vec4 pos;

uniform mat4 M;

void main()
{
    gl_Position=M*pos;
}