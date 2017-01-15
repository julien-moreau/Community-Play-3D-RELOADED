#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

uniform sampler2D ColorMapSampler : registerTexture(t0);
uniform sampler2D LensColorSampler : registerTexture(t1);

SamplerState ColorMapSamplerST : register(s0);
SamplerState LensColorSamplerST : register(s1);

uniform float dispersal;
uniform float haloWidth;
uniform float distortion;

float4 textureDistorted(
	float2 texCoord,
	float2 direction,
	float3 distortion
) {
	return float4(
		CP3DTex2D(ColorMapSampler, texCoord + direction * distortion.r, ColorMapSamplerST).r,
		CP3DTex2D(ColorMapSampler, texCoord + direction * distortion.g, ColorMapSamplerST).g,
		CP3DTex2D(ColorMapSampler, texCoord + direction * distortion.b, ColorMapSamplerST).b,
		1.0
	);
}

float4 pixelMain(VS_OUTPUT In) : COLOR0{
	float2 texCoord = -In.TexCoords.xy + float2(1.0, 1.0);
	float2 texelSize = 1.0 / In.ScreenSize;

	float2 ghostVec = (float2(0.5, 0.5) - texCoord) * dispersal;
	float2 haloVec = normalize(ghostVec) * haloWidth;

	float3 finalDistortion = float3(-texelSize.x * distortion, 0.0, texelSize.x * distortion);

	// Sample ghosts
	float4 result = float4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < SAMPLES; ++i) {
		float2 offset = frac(texCoord + ghostVec * float(i));

		float weight = length(float2(0.5, 0.5) - offset) / length(float2(0.5, 0.5));
		weight = pow(1.0 - weight, 10.0);

		result += textureDistorted(
			offset,
			normalize(ghostVec),
			finalDistortion
		) * weight;
	}

	result *= CP3DTex2D(LensColorSampler, length(float2(0.5, 0.5) - texCoord) / length(float2(0.5, 05)), LensColorSamplerST);

	// Sample halo
	float weight = length(float2(0.5, 0.5) - frac(texCoord + haloVec)) / length(float2(0.5, 05));
	weight = pow(1.0 - weight, 10.0);
	result += textureDistorted(
		frac(texCoord + haloVec),
		normalize(ghostVec),
		finalDistortion
	) * weight;

	return result;
}
