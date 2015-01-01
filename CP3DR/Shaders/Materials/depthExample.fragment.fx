
#include "Shaders/InternalHandler/Utils.hlsl.fx" // Include utils for hlsl

struct VS_OUTPUT
{
	float4 Position : SV_Position;

	#ifdef CP3D_COMPUTE_DEPTH_MATERIAL
	float4 ClipPos  : TEXCOORD0;
	#else
	float4 TexCoord : TEXCOORD0;
	#endif
};

// Declare textures and samplers only if not computing depth
#ifndef CP3D_COMPUTE_DEPTH_MATERIAL
CP3DTexture ColorMapSampler    : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);
#endif

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	#ifdef CP3D_COMPUTE_DEPTH_MATERIAL
	return computeDepthPixel(In.ClipPos);
	#else
	return CP3DTex2D(ColorMapSampler, In.TexCoord.xy, ColorMapSamplerST);
	#endif
}
