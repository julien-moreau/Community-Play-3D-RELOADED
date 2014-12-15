Texture2D ColorMapSampler  : register(t0);
Texture2D ScreenMapSampler : register(t1);

SamplerState ColorMapSamplerST : register(s0);
SamplerState ScreenMapSamplerST : register(s1);

float4 pixelMain(float2 TexCoords : TEXCOORD0) : SV_Target
{
	float4 finalCol = ColorMapSampler.Sample(ColorMapSamplerST, TexCoords);
	float4 lightCol = ScreenMapSampler.Sample(ScreenMapSamplerST, TexCoords);

	return finalCol * lightCol;
}
