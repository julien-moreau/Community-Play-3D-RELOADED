//! Depth material usually used for depth pass
//! and shadow maps

#ifdef OPENGL_DRIVER

uniform mat4 mWorldViewProj;
uniform float MaxD;

void main()
{
	vec4 tPos = mWorldViewProj * gl_Vertex;
	gl_Position = tPos;
	gl_TexCoord[0] = vec4(MaxD, tPos.y, tPos.z, tPos.w);

	gl_TexCoord[1].xy = gl_MultiTexCoord0.xy;
}

#else

float4x4 mWorldViewProj;
float MaxD;

struct VS_OUTPUT 
{
	float4 Position: POSITION;
	float4 ClipPos: TEXCOORD0;
	float2 Texcoords: TEXCOORD1;
	float4 VColor: TEXCOORD2;
};

VS_OUTPUT vertexMain(float3 Position : POSITION0, float2 Texcoords : TEXCOORD0, float4 vColor : COLOR0)
{
	VS_OUTPUT OUT;
	float4 hpos = mul(float4(Position.x, Position.y, Position.z, 1.0), mWorldViewProj);

	OUT.ClipPos = hpos;
	OUT.ClipPos.x = MaxD;
	OUT.Position = hpos;
	OUT.Texcoords = Texcoords;
	OUT.VColor = vColor;

	return OUT;
}

#endif
