
uniform sampler2D U_MainTexture;
varying vec2 V_Texcoord;

void main()
{
	float depthValue=pow(texture2D(U_MainTexture,V_Texcoord).r,8.0);
    gl_FragColor=vec4(vec3(depthValue),1.0);
}