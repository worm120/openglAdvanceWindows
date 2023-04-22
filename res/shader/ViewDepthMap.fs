
void main()
{
	gl_FragColor=vec4(vec3(pow(gl_FragCoord.z,32.0)),1.0);
}