
varying vec3 V_Normal;

void main()
{
	vec3 lightPos=vec3(10.0,10.0,0.0);
	vec3 L=lightPos;
	L=normalize(L);
	vec3 n=normalize(V_Normal);

	//ambient
	vec4 AmbientLightColor=vec4(0.2,0.2,0.2,1.0);
	vec4 AmbientMaterial=vec4(0.2,0.2,0.2,1.0);
	vec4 ambientColor=AmbientLightColor*AmbientMaterial;

	//diffuse
	vec4 DiffuseLightColor=vec4(1.0,1.0,1.0,1.0);
	vec4 DiffuseMaterial=vec4(0.4,0.4,0.4,1.0);
	vec4 diffuseColor=DiffuseLightColor*DiffuseMaterial*max(0.0,dot(L,n));

	//specular
    gl_FragColor=ambientColor+diffuseColor;
}