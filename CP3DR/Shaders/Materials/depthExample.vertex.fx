
#include "Shaders/InternalHandler/Utils.hlsl.fx" // Include utils for hlsl

cbuffer cbParams : register(c0)
{
	float4x4 mWorldViewProj;
	float time;
};

struct VS_INPUT
{
	float3 Position : POSITION;
	float4 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
	float4 Position : SV_Position;

	#if defined(CP3D_COMPUTE_DEPTH_MATERIAL)
	float4 ClipPos  : TEXCOORD0;

	#elif defined(CP3D_COMPUTE_SHADOWS_MATERIAL)
	float4 ShadowMapSamplingPos : TEXCOORD0;
	float4 MVar        			: TEXCOORD1;

	#else
	float4 TexCoord : TEXCOORD0;
	#endif
};

VS_OUTPUT vertexMain(VS_INPUT In)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	float3 p = In.Position;
	p.x += sin(2.0 * p.y + (time)) * 25.0;
	float4 hpos = mul(float4(p.xyz, 1.0), mWorldViewProj);

	OUT.Position = hpos;

	#if defined(CP3D_COMPUTE_DEPTH_MATERIAL)
	OUT.ClipPos = computeDepthVertex(hpos, 600.0);

	#elif defined(CP3D_COMPUTE_SHADOWS_MATERIAL)
	VS_OUTPUT_SHADOWS_MATERIAL outShadows = computeShadowsVertex(hpos);
	OUT.ShadowMapSamplingPos = outShadows.ShadowMapSamplingPos;
	OUT.MVar = outShadows.MVar;

	#else
	OUT.TexCoord = In.TexCoord;

	#endif

	return (OUT);
}
