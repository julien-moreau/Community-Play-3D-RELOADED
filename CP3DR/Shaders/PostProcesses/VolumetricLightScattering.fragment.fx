#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;
uniform sampler2D UserMapSampler;

uniform vec4 lightColor;
uniform vec2 lightPositionOnScreen;
uniform float decay;
uniform float exposure;
uniform float density;
uniform float weight;

void main() 
{		
	const int NUM_SAMPLES = 100;
	vec2 tc = gl_TexCoord[0].xy;

	vec2 deltaTexCoord = (tc - lightPositionOnScreen.xy);
	deltaTexCoord *= 1.0 / NUM_SAMPLES * density;

	float illuminationDecay = 1.0;
	vec4 color = texture2D(UserMapSampler, tc) * 0.4;

	for (int i = 0; i < NUM_SAMPLES; i++) {
		tc -= deltaTexCoord;
		vec4 sample = texture2D(UserMapSampler, tc) * 0.4;
		sample *= illuminationDecay * weight;
		color += lightColor * sample;
		illuminationDecay *= decay;
	}

	vec4 realColor = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
	gl_FragColor = ((vec4((vec3(color.r, color.g, color.b) * exposure), 1)) + (realColor*(1.5 - 0.4)));
}

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
CP3DTexture UserMapSampler : registerTexture(t3);

SamplerState ColorMapSamplerST : register(s0);
SamplerState UserMapSamplerST : register(s3);

float4 lightColor;
float2 lightPositionOnScreen;
float decay;
float exposure;
float density;
float weight;

float4 pixelMain(VS_OUTPUT In) : COLOR0{

	const int NUM_SAMPLES = 100;
	float2 tc = In.TexCoords.xy;

	float2 deltaTexCoord = (tc - lightPositionOnScreen.xy);
	deltaTexCoord *= 1.0f / NUM_SAMPLES * density;

	float illuminationDecay = 1.0;
	float4 color = CP3DTex2D(UserMapSampler, tc, UserMapSamplerST) * 0.4;

	for (int i = 0; i < NUM_SAMPLES; i++) {
		tc -= deltaTexCoord;
		float4 sample = CP3DTex2D(UserMapSampler, tc, UserMapSamplerST) * 0.4;
		sample *= illuminationDecay * weight;
		color += lightColor * sample;
		illuminationDecay *= decay;
	}

	float4 realColor = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST);
	return ((float4((float3(color.r, color.g, color.b) * exposure), 1)) + (realColor*(1.5 - 0.4)));
}

#endif
