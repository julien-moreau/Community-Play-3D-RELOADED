#ifdef OPENGL_DRIVER
uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;
uniform sampler2D DepthMapSampler;
uniform sampler2D UserMapSampler; // Lens star
uniform sampler2D UserMapSampler2; // Lens dirt

uniform float motionScale;

uniform mat4 inverseViewProjection;
uniform mat4 prevViewProjection;
uniform mat4 lensStarMatrix;

vec4 motionBlur(vec2 texCoord, vec2 screenSize) {
	float depth = texture2D(DepthMapSampler, texCoord).r;

	vec4 cpos = vec4(texCoord * 2.0 - 1.0, depth, 1.0);
	cpos = cpos * inverseViewProjection;

	vec4 ppos = cpos * prevViewProjection;
	ppos.xyz /= ppos.w;
	ppos.xy = ppos.xy * 0.5 + 0.5;

	vec2 velocity = (ppos.xy - texCoord) * motionScale;
	vec2 texelSize = 1.0 / screenSize;

	float tspeed = length(velocity / texelSize); // speed in pixels/second
	int nSamples = int(clamp(tspeed, 1.0, MAX_MOTION_SAMPLES));

	vec4 result = texture2D(ScreenMapSampler, texCoord);

	[loop]
	for (int i = 1; i < nSamples; ++i) {
		vec2 offset = texCoord + velocity * (float(i) / float(nSamples - 1) - 0.5);
		result += texture2D(ScreenMapSampler, offset);
	}

	return result / float(nSamples);
}

void main()
{
	vec4 lensMod = texture2D(UserMapSampler2, gl_TexCoord[0].xy);

	vec2 lensStarTexcoord = (lensStarMatrix * vec4(gl_TexCoord[0].xy, 1.0, 1.0)).xy;
	lensMod += texture2D(UserMapSampler, lensStarTexcoord);

	vec4 lensColor = texture2D(ColorMapSampler, gl_TexCoord[0].xy);

	gl_FragColor = motionBlur(gl_TexCoord[0].xy, gl_TexCoord[3].xy) + lensColor * lensMod;
}

#else
#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
CP3DTexture ScreenMapSampler : registerTexture(t1);
CP3DTexture DepthMapSampler : registerTexture(t2);
CP3DTexture LensStarSampler : registerTexture(t3);
CP3DTexture LensDirtSampler : registerTexture(t4);

#ifdef OBJECT_BASED_MOTION_BLUR
CP3DTexture VelocitySampler : registerTexture(t5);
#endif

SamplerState ColorMapSamplerST : register(s0);
SamplerState ScreenMapSamplerST : register(s1);
SamplerState DepthMapSamplerST : register(s2);
SamplerState LensStarSamplerST : register(s3);
SamplerState LensDirtSamplerST : register(s4);

#ifdef OBJECT_BASED_MOTION_BLUR
SamplerState VelocitySamplerST : register(s5);
#endif

float motionScale;

#ifndef OBJECT_BASED_MOTION_BLUR
float4x4 inverseViewProjection;
float4x4 prevViewProjection;
#endif

float4x4 lensStarMatrix;

inline float4 motionBlur(float2 texCoord, float2 screenSize) {
	// Common
	float2 texelSize = 1.0 / screenSize;
	int nSamples = 0;

	#ifdef OBJECT_BASED_MOTION_BLUR
	// Velocity
	float4 result = float4(0.0, 0.0, 0.0, 0.0);

	float2 velocity = pow(CP3DTex2D(VelocitySampler, texCoord, VelocitySamplerST).rg, 1.0 / 3.0) * 2.0 - 1.0;
	velocity *= motionScale;

	float speed = length(velocity / texelSize);
	nSamples = clamp(int(speed), 1.0, MAX_MOTION_SAMPLES);

	velocity = normalize(velocity) * texelSize;
	
	float hlim = float(-nSamples) * 0.5 + 0.5;

	for (int i = 0; i < nSamples; ++i) {
		float2 offset = velocity * (hlim + float(i));
		result += CP3DTex2D(ScreenMapSampler, texCoord + offset, ScreenMapSamplerST);
	}

	result /= float(nSamples);

	return result;
	#else

	// Motion blur
	float4 result = float4(0.0, 0.0, 0.0, 0.0);

	float depth = CP3DTex2D(DepthMapSampler, texCoord, DepthMapSamplerST).r;

	float4 cpos = float4(texCoord * 2.0 - 1.0, depth, 1.0);
	cpos = mul(inverseViewProjection, cpos);

	float4 ppos = mul(prevViewProjection, cpos);
	ppos.xyz /= ppos.w;
	ppos.xy = ppos.xy * 0.5 + 0.5;

	float2 velocity = (ppos.xy - texCoord) * motionScale;

	float speed = length(velocity / texelSize); // speed in pixels/second
	nSamples = clamp(speed, 1.0, MAX_MOTION_SAMPLES);

	result = CP3DTex2D(ScreenMapSampler, texCoord, ScreenMapSamplerST);

	for (int i = 1; i < nSamples; ++i) {
		float2 offset = texCoord + velocity * (float(i) / float(nSamples - 1) - 0.5);
		result += CP3DTex2D(ScreenMapSampler, offset, ScreenMapSamplerST);
	}

	result /= float(nSamples);

	return result;
	#endif
}

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	float4 lensMod = CP3DTex2D(LensDirtSampler, In.TexCoords.xy, LensDirtSamplerST);

	float2 lensStarTexcoord = mul(lensStarMatrix, float4(In.TexCoords.xy, 1.0, 1.0)).xy;
	lensMod += CP3DTex2D(LensStarSampler, lensStarTexcoord, LensStarSamplerST);

	float4 lensColor = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST);

	return motionBlur(In.TexCoords.xy, In.ScreenSize) + lensColor * lensMod;
}
#endif
