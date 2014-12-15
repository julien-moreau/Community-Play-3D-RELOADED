Texture2D ColorMapSampler  : register(t0);
Texture2D ScreenMapSampler : register(t1);

SamplerState ColorMapSamplerST : register(s0);
SamplerState ScreenMapSamplerST : register(s1);

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float2 TexCoords	: TEXCOORD0;
	float3 LStart		: TEXCOORD1;
	float3 LEnd			: TEXCOORD2;
	float2 ScreenSize	: TEXCOORD3;
};

float4 pixelMain(VS_OUTPUT In) : SV_TARGET
{
	float4 finalCol = ColorMapSampler.Sample(ColorMapSamplerST, In.TexCoords);
	float4 lightCol = ScreenMapSampler.Sample(ScreenMapSamplerST, In.TexCoords);

	return finalCol * lightCol;
}
