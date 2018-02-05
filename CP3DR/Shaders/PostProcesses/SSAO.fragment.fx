#ifdef OPENGL_DRIVER

#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D DepthMapSampler;
uniform sampler2D UserMapSampler;

vec3 normalFromDepth(float depth, vec2 coords) {
	const vec2 offset1 = vec2(0.0, 0.001);
	const vec2 offset2 = vec2(0.001, 0.0);

	float depth1 = texture2D(DepthMapSampler, coords + offset1).r;
	float depth2 = texture2D(DepthMapSampler, coords + offset2).r;

	vec3 p1 = vec3(offset1, depth1 - depth);
	vec3 p2 = vec3(offset2, depth2 - depth);

	vec3 normal = cross(p1, p2);
	normal.z = -normal.z;

	return normalize(normal);
}

void main()
{
	const float totalStrength = 2.0;
	const float base = 1.0;

	const float area = 1.0;
	const float fallOff = 0.000001;

	const float radius = 0.001;

	const int samples = 16;

	vec3 sampleSphere[samples];
	sampleSphere[0] = vec3(0.5381, 0.1856, -0.4319);
	sampleSphere[1] = vec3(0.1379, 0.2486, 0.4430);
	sampleSphere[2] = vec3(0.3371, 0.5679, -0.0057);
	sampleSphere[3] = vec3(-0.6999, -0.0451, -0.0019);
	sampleSphere[4] = vec3(0.0689, -0.1598, -0.8547);
	sampleSphere[5] = vec3(0.0560, 0.0069, -0.1843);
	sampleSphere[6] = vec3(-0.0146, 0.1402, 0.0762);
	sampleSphere[7] = vec3(0.0100, -0.1924, -0.0344);
	sampleSphere[8] = vec3(-0.3577, -0.5301, -0.4358);
	sampleSphere[9] = vec3(-0.3169, 0.1063, 0.0158);
	sampleSphere[10] = vec3(0.0103, -0.5869, 0.0046);
	sampleSphere[11] = vec3(-0.0897, -0.4940, 0.3287);
	sampleSphere[12] = vec3(0.7119, -0.0154, -0.0918);
	sampleSphere[13] = vec3(-0.0533, 0.0596, -0.5411);
	sampleSphere[14] = vec3(0.0352, -0.0631, 0.5460);
	sampleSphere[15] = vec3(-0.4776, 0.2847, -0.0271);

	vec3 random = normalize(texture2D(UserMapSampler, gl_TexCoord[0].xy * 4.0).rgb);
	float depth = texture2D(DepthMapSampler, gl_TexCoord[0].xy).r;

	vec3 position = vec3(gl_TexCoord[0].xy, depth);
	vec3 normal = normalFromDepth(depth, gl_TexCoord[0].xy);

	float radiusDepth = radius / depth;
	float occlusion = 0.0;

	for (int i = 0; i < samples; i++) {
		vec3 ray = radiusDepth * reflect(sampleSphere[i] * 16.0, random);
		vec3 hemiRay = position + sign(dot(ray, normal)) * ray;

		float occlusionDepth = texture2D(DepthMapSampler, clamp(hemiRay.xy, 0.0, 1.0)).r;
		float difference = depth - occlusionDepth;

		occlusion += step(fallOff, difference) * (1.0 - smoothstep(fallOff, area, difference));
	}

	float ao = 1.0 - totalStrength * occlusion * (1.0 / float(samples));

	float result = clamp(ao + base, 0.0, 1.0);
	gl_FragColor.r = result;
	gl_FragColor.g = result;
	gl_FragColor.b = result;
	gl_FragColor.a = 1.0;
}

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture NormalTextureSampler : registerTexture(t1);
CP3DTexture DepthTextureSampler : registerTexture(t2);
CP3DTexture RandomTextureSampler : registerTexture(t3);

SamplerState NormalTextureSamplerST : register(s1);
SamplerState DepthTextureSamplerST  : register(s2);
SamplerState RandomTextureSamplerST : register(s3);

float3 normal_from_depth(float depth, float2 texcoords) {
	const float2 offset1 = float2(0.0, 0.001);
	const float2 offset2 = float2(0.001, 0.0);

	float depth1 = CP3DTex2D(DepthTextureSampler, texcoords + offset1, DepthTextureSamplerST).r;
	float depth2 = CP3DTex2D(DepthTextureSampler, texcoords + offset2, DepthTextureSamplerST).r;

	float3 p1 = float3(offset1, depth1 - depth);
	float3 p2 = float3(offset2, depth2 - depth);

	float3 normal = cross(p1, p2);
	normal.z = -normal.z;

	return normalize(normal);

	/*
	float3 normal = CP3DTex2D(NormalTextureSampler, texcoords, NormalTextureSamplerST).rgb;
	normal.z = -normal.z;

	return normalize(normal);
	*/
}

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	const float total_strength = 1.0; // 1.0
	const float base = 0.2; // 0.2

	const float area = 1.0; //0.0075
	const float falloff = 0.00001; //0.000001

	const float radius = 0.01; //0.0002
	const int samples = 16;
	float3 sample_sphere[16] = {
		float3(0.5381, 0.1856, -0.4319), float3(0.1379, 0.2486, 0.4430),
		float3(0.3371, 0.5679, -0.0057), float3(-0.6999, -0.0451, -0.0019),
		float3(0.0689, -0.1598, -0.8547), float3(0.0560, 0.0069, -0.1843),
		float3(-0.0146, 0.1402, 0.0762), float3(0.0100, -0.1924, -0.0344),
		float3(-0.3577, -0.5301, -0.4358), float3(-0.3169, 0.1063, 0.0158),
		float3(0.0103, -0.5869, 0.0046), float3(-0.0897, -0.4940, 0.3287),
		float3(0.7119, -0.0154, -0.0918), float3(-0.0533, 0.0596, -0.5411),
		float3(0.0352, -0.0631, 0.5460), float3(-0.4776, 0.2847, -0.0271)
	};
	
	float3 random = normalize(CP3DTex2D(RandomTextureSampler, In.TexCoords.xy * 4.0, RandomTextureSamplerST).rgb);

	float depth = CP3DTex2D(DepthTextureSampler, In.TexCoords.xy, DepthTextureSamplerST).r;

	float3 position = float3(In.TexCoords.xy, depth);
	float3 normal = normal_from_depth(depth, In.TexCoords.xy);

	float radius_depth = radius * depth;
	float occlusion = 0.0;
	for (int i = 0; i < samples; i++) {

		float3 ray = radius_depth * reflect(sample_sphere[i] * 16.0, random);
		float3 hemi_ray = position + sign(dot(ray, normal)) * ray;

		float occ_depth = CP3DTex2D(DepthTextureSampler, saturate(hemi_ray.xy), DepthTextureSamplerST).r;
		float difference = depth - occ_depth;

		occlusion += step(falloff, difference) * (1.0 - smoothstep(falloff, area, difference));
	}

	float ao = 1.0 - total_strength * occlusion * (1.0 / samples);
	float4 Output = saturate(ao + base);
	Output.a = 1.0;

	return Output;
}

#endif

//http://www.babylonjs-playground.com/#Y7RCM#1
