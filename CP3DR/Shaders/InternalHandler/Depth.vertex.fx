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
