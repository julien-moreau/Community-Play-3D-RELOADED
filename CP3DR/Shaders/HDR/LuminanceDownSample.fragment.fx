#ifdef OPENGL_DRIVER

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

float2 dsOffsets[9];
float halfDestPixelSize;

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	float4 color = 0.0;
	float average = 0.0;

	for (int i = 0; i < 9; i++)
	{
		color = CP3DTex2D(ColorMapSampler, In.TexCoords.xy + float2(halfDestPixelSize, halfDestPixelSize) + dsOffsets[i], ColorMapSamplerST);
		average += color.r;
	}

	average /= 9.0f;

	return average;
}

#endif
