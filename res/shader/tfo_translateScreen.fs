precision mediump float;
uniform sampler2D U_MainTexture;
varying vec4 V_Color;
varying vec2 V_Texcoord;

void main()
{
    gl_FragColor=vec4(vec3(0.1,0.4,0.7),texture2D(U_MainTexture,V_Texcoord).a*V_Color.a);
}