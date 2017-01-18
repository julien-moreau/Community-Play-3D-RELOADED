#ifdef OPENGL_DRIVER
uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

uniform float dispersal;
uniform float haloWidth;
uniform float distortion;

vec4 textureDistorted(
	vec2 texCoord,
	vec2 direction,
	vec3 distortion
) {
	return vec4(
		texture2D(ColorMapSampler, texCoord + direction * distortion.r).r,
		texture2D(ColorMapSampler, texCoord + direction * distortion.g).g,
		texture2D(ColorMapSampler, texCoord + direction * distortion.b).b,
		1.0
		);
}

void main()
{
	vec2 texCoord = -gl_TexCoord[0].xy + vec2(1.0, 1.0);
	vec2 texelSize = 1.0 / gl_TexCoord[3].xy;

	vec2 ghostVec = (vec2(0.5, 0.5) - texCoord) * dispersal;
	vec2 haloVec = normalize(ghostVec) * haloWidth;

	vec3 finalDistortion = vec3(-texelSize.x * distortion, 0.0, texelSize.x * distortion);

	// Sample ghosts
	vec4 result = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < SAMPLES; ++i) {
		vec2 offset = fract(texCoord + ghostVec * float(i));

		float weight = length(vec2(0.5, 0.5) - offset) / length(vec2(0.5, 0.5));
		weight = pow(1.0 - weight, 10.0);

		result += textureDistorted(
			offset,
			normalize(ghostVec),
			finalDistortion
		) * weight;
	}

	result *= texture2D(ScreenMapSampler, vec2(length(vec2(0.5, 0.5) - texCoord) / length(vec2(0.5, 05))));

	// Sample halo
	float weight = length(vec2(0.5, 0.5) - fract(texCoord + haloVec)) / length(vec2(0.5, 05));
	weight = pow(1.0 - weight, 10.0);
	result += textureDistorted(
		fract(texCoord + haloVec),
		normalize(ghostVec),
		finalDistortion
	) * weight;

	gl_FragColor = result;
}
#else

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
#endif
