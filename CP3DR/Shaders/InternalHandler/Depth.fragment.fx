##ifdef ALPHA_ENABLED
##ifdef DIRECT3D_11
	Texture2D ColorMapSampler : register(t0);
	SamplerState ColorMapSamplerST : register(s0);
##else
	sampler2D ColorMapSampler : register(s0);
##endif
##endif

struct VS_OUTPUT
{
	float4 Position: POSITION0;
	float4 ClipPos: TEXCOORD0;
	float2 Texcoords: TEXCOORD1;
	float4 VColor: TEXCOORD2;
};

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float depth = In.ClipPos.z / In.ClipPos.x;
	float alpha = 0.0;

##ifdef ALPHA_ENABLED
##ifdef DIRECT3D_11
		alpha = ColorMapSampler.Sample(ColorMapSamplerST, In.Texcoords).a;
##else
		alpha = tex2D(ColorMapSampler, In.Texcoords).a;
##endif
##endif

	return float4(depth, depth * depth, 0.0, alpha);
}
