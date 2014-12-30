
cbuffer cbParams : register(c0)
{
	float4x4 mWorldViewProj;
};

struct VS_INPUT
{
	float3 Position : POSITION;
	float4 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
	float4 Position : SV_Position;
	float4 TexCoord : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT  OUT;
	OUT.Position = mul(float4(input.Position.xyz, 1.0), mWorldViewProj);
	OUT.TexCoord = input.TexCoord;
	return (OUT);
}
