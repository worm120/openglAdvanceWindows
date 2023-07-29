precision mediump float;
uniform sampler2D U_MainTexture;
varying vec4 V_Color;

void main()
{
    gl_FragColor=vec4(vec3(1.0,0.6,0.2),texture2D(U_MainTexture,gl_PointCoord.xy).a*V_Color.a);
}