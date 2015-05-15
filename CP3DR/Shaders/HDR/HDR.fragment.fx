#ifdef OPENGL_DRIVER

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

float exposure;
float averageLuminance;

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	float4 color = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST);
	float4 adjustedColor = color / averageLuminance * exposure;

	color = adjustedColor;
	color.a = 1.0;
	return color;
}

#endif
