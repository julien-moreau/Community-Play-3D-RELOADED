
#ifdef OPENGL_DRIVER

uniform mat4 mWorldViewProj;

void main(void) {
	vec4 tPos = mWorldViewProj * gl_Vertex;
	gl_Position = tPos;
	gl_TexCoord[0] = tPos;
}

#else

float4x4 mWorldViewProj;

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float4 ClipPos: TEXCOORD0;
	float2 Texcoords: TEXCOORD1;
	float4 VColor: TEXCOORD2;
};

VS_OUTPUT vertexMain(float4 Position : POSITION, float2 Texcoords : TEXCOORD0, float4 vColor : COLOR0)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	float4 hpos = mul(Position, mWorldViewProj);
	OUT.Position = hpos;
	OUT.ClipPos = hpos;
	OUT.Texcoords = Texcoords;
	OUT.VColor = vColor;

	return (OUT);
}

#endif
