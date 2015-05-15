
#ifdef OPENGL_DRIVER

uniform sampler2D diffuseSampler;
#ifdef _CP3D_SOLID_2_LAYER_
uniform sampler2D blendSampler;
#endif

void main(void) {
	vec4 diffuseColor = texture2D(diffuseSampler, gl_TexCoord[0].xy);

	#ifdef _CP3D_SOLID_2_LAYER_
	gl_FragColor = diffuseColor * texture2D(blendSampler, gl_TexCoord[0].xy);
	#else
	gl_FragColor = diffuseColor;
	#endif
}

#else

#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture	waterBump : register(t0);
CP3DTexture	refractionMap : register(t1);
CP3DTexture	reflectionMap : register(t2);

SamplerState waterBumpST : register(s0);
SamplerState refractionMapST : register(s1);
SamplerState reflectionMapST : register(s2);

float3 cameraPosition;
float waveHeight;
float4 waterColor;
float colorBlendFactor;

struct VS_OUTPUT
{
	float4 Position				 : SV_Position;
	float2 BumpMapTexCoord		 : TEXCOORD0;
	float3 RefractionMapTexCoord : TEXCOORD1;
	float3 ReflectionMapTexCoord : TEXCOORD2;
	float3 Position3D			 : TEXCOORD3;
};

float4 pixelMain(VS_OUTPUT input) : COLOR0
{
	float4 bumpColor = CP3DTex2D(waterBump, input.BumpMapTexCoord, waterBumpST);
	float2 perturbation = waveHeight * (bumpColor.rg - 0.5);

	float2 projectedRefractionTexCoords = saturate(input.RefractionMapTexCoord.xy / input.RefractionMapTexCoord.z + perturbation);
	float4 refractiveColor = CP3DTex2D(refractionMap, projectedRefractionTexCoords, refractionMapST);

	float2 projectedReflectionTexCoords = saturate(input.ReflectionMapTexCoord.xy / input.ReflectionMapTexCoord.z + perturbation);
	float4 reflectiveColor = CP3DTex2D(reflectionMap, projectedReflectionTexCoords, reflectionMapST);

	float3 eyeVector = normalize(cameraPosition - input.Position3D);
	float3 upVector = float3(0.0, 1.0, 0.0);

	float fresnelTerm = max(dot(eyeVector, upVector), 0.0);
	float4 combinedColor = refractiveColor * fresnelTerm + reflectiveColor * (1.0 - fresnelTerm);

	return colorBlendFactor * waterColor + (1.0 - colorBlendFactor) * combinedColor;
}

#endif
