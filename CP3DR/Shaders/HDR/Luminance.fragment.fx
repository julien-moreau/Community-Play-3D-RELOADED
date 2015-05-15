#ifdef OPENGL_DRIVER

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

float2 lumOffsets[4];

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	float average = 0.0;
	float4 color = 0.0;
	float maximum = -1e20;
	float3 weight = float3(0.299, 0.587, 0.114);

	for (int i = 0; i < 4; i++) {
		color = CP3DTex2D(ColorMapSampler, In.TexCoords.xy + lumOffsets[i], ColorMapSamplerST);

		#ifdef SIMPLE
		float GreyValue = dot(color.rgb, float3(0.33, 0.33, 0.33));
		#endif

		#ifdef WEIGHTED_AVERAGE
		float GreyValue = dot(color.rgb, weight);
		#endif

		#ifdef BRIGHTNESS
		float GreyValue = max(color.r, max(color.g, color.b));
		#endif

		#ifdef HSL_COMPONENT
		float GreyValue = 0.5 * (max(color.r, max(color.g, color.b)) + min(color.r, min(color.g, color.b)));
		#endif

		#ifdef MAGNITUDE
		float GreyValue = length(color.rgb);
		#endif

		maximum = max(maximum, GreyValue);
		average += (0.25f * log(1e-5 + GreyValue));
	}

	average = exp(average);

	return float4(average, maximum, 0.0, 1.0);
}

#endif
