attribute vec4 pos;

void main()
{
    gl_Position=pos+vec4(0.0,0.001,0.0,0.0);
}