uniform sampler2D U_MainTexture;

void main()
{
    gl_FragColor=texture2D(U_MainTexture,gl_PointCoord.xy);
}