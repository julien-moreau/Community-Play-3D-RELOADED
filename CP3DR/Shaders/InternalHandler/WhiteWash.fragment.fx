##ifdef DIRECT3D_11
Texture2D ColorMapSampler  : register(t0);

SamplerState ColorMapSamplerST : register(s0);
##else
sampler2D ColorMapSampler : register(s0);
##endif

float4 pixelMain(float2 TexCoords : TEXCOORD0) : COLOR0
{
##ifdef DIRECT3D_11
	float4 finalCol = ColorMapSampler.Sample(ColorMapSamplerST, In.TexCoords);
##else
	float4 finalCol = tex2D(ColorMapSampler, TexCoords);
##endif

	return finalCol;
}