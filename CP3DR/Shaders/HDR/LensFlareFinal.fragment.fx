#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

uniform sampler2D ColorMapSampler : registerTexture(t0);
uniform sampler2D ScreenMapSampler : registerTexture(t1);
uniform sampler2D LensStarSampler : registerTexture(t2);
uniform sampler2D LensDirtSampler : registerTexture(t3);
uniform sampler2D DepthMapSampler : registerTexture(t4);

SamplerState ColorMapSamplerST : register(s0);
SamplerState ScreenMapSamplerST : register(s1);
SamplerState LensStarSamplerST : register(s2);
SamplerState LensDirtSamplerST : register(s3);
SamplerState DepthMapSamplerST : register(s4);

uniform float motionScale;

uniform float4x4 inverseViewProjection;
uniform float4x4 prevViewProjection;
uniform float4x4 lensStarMatrix;

inline float4 motionBlur(float2 texCoord, float2 screenSize) {
	float depth = CP3DTex2D(DepthMapSampler, texCoord, DepthMapSamplerST).r;

	float4 cpos = float4(texCoord * 2.0 - 1.0, depth, 1.0);
	cpos = mul(inverseViewProjection, cpos);

	float4 ppos = mul(prevViewProjection, cpos);
	ppos.xyz /= ppos.w;
	ppos.xy = ppos.xy * 0.5 + 0.5;

	float2 velocity = (ppos.xy - texCoord) * motionScale;
	float2 texelSize = 1.0 / screenSize;

	float tspeed = length(velocity / texelSize); // speed in pixels/second
	int nSamples = clamp(tspeed, 1.0, MAX_MOTION_SAMPLES);

	float4 result = CP3DTex2D(ScreenMapSampler, texCoord, ScreenMapSamplerST);
	for (int i = 1; i < nSamples; ++i) {
		float2 offset = texCoord + velocity * (float(i) / float(nSamples - 1) - 0.5);
		result += CP3DTex2D(ScreenMapSampler, offset, ScreenMapSamplerST);
	}

	return result / float(nSamples);
}

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	
	float4 lensMod = CP3DTex2D(LensDirtSampler, In.TexCoords.xy, LensDirtSamplerST);

	float2 lensStarTexcoord = mul(lensStarMatrix, float4(In.TexCoords.xy, 1.0, 1.0)).xy;
	lensMod += CP3DTex2D(LensStarSampler, lensStarTexcoord, LensStarSamplerST);

	float4 lensColor = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST);

	return motionBlur(In.TexCoords.xy, In.ScreenSize) + lensColor * lensMod;
}
