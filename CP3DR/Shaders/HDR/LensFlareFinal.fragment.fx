#ifdef OPENGL_DRIVER
uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;
uniform sampler2D DepthMapSampler;
uniform sampler2D UserMapSampler; // Lens star
uniform sampler2D UserMapSampler2; // Lens dirt
uniform sampler2D UserMapSampler3; // Velocity

uniform float motionScale;

uniform mat4 inverseViewProjection;
uniform mat4 prevViewProjection;
uniform mat4 lensStarMatrix;

vec4 motionBlur(vec2 texCoord, vec2 screenSize) {
	// Common
	vec2 texelSize = 1.0 / screenSize;

	// Motion blur
	float depth = texture2D(DepthMapSampler, texCoord).r;

	vec4 cpos = vec4(texCoord * 2.0 - 1.0, depth, 1.0);
	cpos = cpos * inverseViewProjection;

	vec4 ppos = cpos * prevViewProjection;
	ppos.xyz /= ppos.w;
	ppos.xy = ppos.xy * 0.5 + 0.5;

	vec2 velocity1 = (ppos.xy - texCoord) * motionScale;
	float speed = length(velocity1 / texelSize);
	int nSamples1 = int(clamp(speed, 1.0, MAX_MOTION_SAMPLES));

	// Object based motion blur
	vec2 velocityColor = texture2D(UserMapSampler3, texCoord).rg;
	vec2 velocity2 = vec2(pow(velocityColor.r, 1.0 / 3.0), pow(velocityColor.g, 1.0 / 3.0)) * 2.0 - 1.0;
	velocity2 *= motionScale;

	speed = length(velocity2 / texelSize);
	int nSamples2 = int(clamp(int(speed), 1.0, MAX_MOTION_SAMPLES));

	velocity2 = normalize(velocity2) * texelSize;
	float hlim = float(-nSamples2) * 0.5 + 0.5;

	// Render
	vec4 result = texture2D(ScreenMapSampler, texCoord);
	int nSamples = max(nSamples1, nSamples2);

	if (nSamples1 > nSamples2) {
		for (int i = 1; i < nSamples; ++i) {
			vec2 offset1 = texCoord + velocity1 * (float(i) / float(nSamples1 - 1) - 0.5);
			result += texture2D(ScreenMapSampler, offset1);
		}
	}
	else {
		for (int i = 1; i < nSamples; ++i) {
			vec2 offset2 = texCoord + velocity2 * (hlim + float(i));
			result += texture2D(ScreenMapSampler, offset2);
		}
	}

	result /= float(nSamples);

	return result;
	/*
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

	for (int i = 1; i < nSamples; ++i) {
		vec2 offset = texCoord + velocity * (float(i) / float(nSamples - 1) - 0.5);
		result += texture2D(ScreenMapSampler, offset);
	}

	return result / float(nSamples);
	*/
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
CP3DTexture VelocitySampler : registerTexture(t5);

SamplerState ColorMapSamplerST : register(s0);
SamplerState ScreenMapSamplerST : register(s1);
SamplerState DepthMapSamplerST : register(s2);
SamplerState LensStarSamplerST : register(s3);
SamplerState LensDirtSamplerST : register(s4);
SamplerState VelocitySamplerST : register(s5);

float motionScale;

float4x4 inverseViewProjection;
float4x4 prevViewProjection;
float4x4 lensStarMatrix;

inline float4 motionBlur(float2 texCoord, float2 screenSize) {
	// Common
	float2 texelSize = 1.0 / screenSize;

	// Motion blur
	float depth = CP3DTex2D(DepthMapSampler, texCoord, DepthMapSamplerST).r;

	float4 cpos = float4(texCoord * 2.0 - 1.0, depth, 1.0);
	cpos = mul(inverseViewProjection, cpos);

	float4 ppos = mul(prevViewProjection, cpos);
	ppos.xyz /= ppos.w;
	ppos.xy = ppos.xy * 0.5 + 0.5;

	float2 velocity1 = (ppos.xy - texCoord) * motionScale;
	float speed = length(velocity1 / texelSize);
	int nSamples1 = clamp(speed, 1.0, MAX_MOTION_SAMPLES);

	// Object based motion blur
	float2 velocity2 = pow(CP3DTex2D(VelocitySampler, texCoord, VelocitySamplerST).rg, 1.0 / 3.0) * 2.0 - 1.0;
	velocity2 *= motionScale;

	speed = length(velocity2 / texelSize);
	int nSamples2 = clamp(int(speed), 1.0, MAX_MOTION_SAMPLES);

	velocity2 = normalize(velocity2) * texelSize;
	float hlim = float(-nSamples2) * 0.5 + 0.5;

	// Render
	float4 result = CP3DTex2D(ScreenMapSampler, texCoord, ScreenMapSamplerST);
	int nSamples = max(nSamples1, nSamples2);

	if (nSamples1 > nSamples2) {
		[loop]
		for (int i = 1; i < nSamples; ++i) {
			float2 offset1 = texCoord + velocity1 * (float(i) / float(nSamples1 - 1) - 0.5);
			result += CP3DTex2D(ScreenMapSampler, offset1, ScreenMapSamplerST);
		}
	}
	else {
		[loop]
		for (int i = 1; i < nSamples; ++i) {
			float2 offset2 = texCoord + velocity2 * (hlim + float(i));
			result += CP3DTex2D(ScreenMapSampler, offset2, ScreenMapSamplerST);
		}
	}

	result /= float(nSamples);

	return result;
}

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	float4 lensMod = CP3DTex2D(LensDirtSampler, In.TexCoords.xy, LensDirtSamplerST);

	float2 lensStarTexcoord = mul(lensStarMatrix, float4(In.TexCoords.xy, 1.0, 1.0)).xy;
	lensMod += CP3DTex2D(LensStarSampler, lensStarTexcoord, LensStarSamplerST);

	float4 lensColor = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST);

	return motionBlur(In.TexCoords.xy, In.ScreenSize) + lensColor * lensMod;
}
#endif
