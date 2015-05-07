#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;
uniform sampler2D DepthMapSampler;

void main() 
{		
	vec4 screenCol = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);
	vec4 depthCol = texture2D(DepthMapSampler, gl_TexCoord[0].xy);
	vec4 SSAOCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy) * ((depthCol.r == 1.0) ? 1.0 : 5.0);

	gl_FragColor = (screenCol * SSAOCol);
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

	float4 realColor = tex2D(ColorMapSampler, In.TexCoords.xy);
	return ((float4((float3(color.r, color.g, color.b) * exposure), 1)) + (realColor*(1.5 - 0.4)));
}

#endif
