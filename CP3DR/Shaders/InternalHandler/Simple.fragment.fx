##ifdef DIRECT3D_11
	Texture2D ColorMapSampler  : register(t0);
	SamplerState ColorMapSamplerST : register(s0);
##else
	sampler2D ColorMapSampler : register(s0);
##endif

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float2 TexCoords	: TEXCOORD0;
	float3 LStart		: TEXCOORD1;
	float3 LEnd			: TEXCOORD2;
	float2 ScreenSize	: TEXCOORD3;
};

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
##ifdef DIRECT3D_11
	float4 finalCol = ColorMapSampler.Sample(ColorMapSamplerST, In.TexCoords);
##else
	float4 finalCol = tex2D(ColorMapSampler, In.TexCoords);
##endif

	return finalCol;
}