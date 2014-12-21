##ifdef DIRECT3D_11
	Texture2D ColorMapSampler : register(t0);
	SamplerState ColorMapSamplerST : register(s0);
##else
	sampler2D ColorMapSampler : register(s0);
##endif

struct VS_OUTPUT
{
	float4 Position		: SV_Position;
	float2 TexCoords	: TEXCOORD0;
	float3 LStart		: TEXCOORD1;
	float3 LEnd			: TEXCOORD2;
	float2 ScreenSize	: TEXCOORD3;
};

float4 pixelMain (VS_OUTPUT In) : COLOR0
{
	float2 offsetArray[5];
##ifdef VERTICAL_VSM_BLUR
	offsetArray[0] = float2(0, 0);
	offsetArray[1] = float2(0, 1.5 / In.ScreenSize.y);
	offsetArray[2] = float2(0, -1.5 / In.ScreenSize.y);
	offsetArray[3] = float2(0, 2.5 / In.ScreenSize.y);
	offsetArray[4] = float2(0, -2.5 / In.ScreenSize.y);
##else
	offsetArray[0] = float2(0, 0);
	offsetArray[1] = float2(1.5 / In.ScreenSize.x, 0);
	offsetArray[2] = float2(-1.5 / In.ScreenSize.x, 0);
	offsetArray[3] = float2(2.5 / In.ScreenSize.x, 0);
	offsetArray[4] = float2(-2.5 / In.ScreenSize.x, 0);
##endif

	float4 finalVal = float4(0.0, 0.0, 0.0, 0.0);

		for (int i = 0; i < 5; ++i) {
##ifdef DIRECT3D_11
			finalVal += ColorMapSampler.Sample(ColorMapSamplerST, clamp(In.TexCoords.xy + offsetArray[i], float2(0.001, 0.001), float2(0.999, 0.999)));
##else
			finalVal += tex2D(ColorMapSampler, clamp(In.TexCoords.xy + offsetArray[i], float2(0.001, 0.001), float2(0.999, 0.999)));
##endif
		}

	return finalVal / 5.0;
}
