
#ifdef OPENGL_DRIVER


#else

#include "Shaders/InternalHandler/Utils.hlsl.fx" // Include utils for hlsl
//#define MAX_JOINT_NUM 57

cbuffer cbParams : register(b0)
{
	float4x4 mWorldViewProj;
	float4x4 JointTransform[MAX_JOINT_NUM];
};

struct VS_INPUT
{
	float3 Position		: POSITION0;
	float2 TexCoords	: TEXCOORD0;
	float4 Color		: COLOR0;

	#if defined(CP3D_COMPUTE_SHADOWS_MATERIAL)
	float3 Normal		: NORMAL;
	#endif

	#if !defined(CP3D_COMPUTE_SHADOWS_MATERIAL) && !defined(CP3D_COMPUTE_DEPTH_MATERIAL)
	float2 TexCoords2	: TEXCOORD1;
	#endif
};

struct VS_OUTPUT
{
	float4 Position		: SV_Position;

	#if defined(CP3D_COMPUTE_DEPTH_MATERIAL) || defined(CP3D_COMPUTE_SHADOWS_MATERIAL)
	float4 ClipPos 		: TEXCOORD0;
	#endif

	#if defined(CP3D_COMPUTE_SHADOWS_MATERIAL)
	float2 TexCoords2 	: TEXCOORD1;
	#endif
};

VS_OUTPUT vertexMain(VS_INPUT input)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	int verCol = int(input.Color.r * 255.9);
	float4x4 matTran = JointTransform[verCol - 1];
	float4 MeshPos = float4(input.Position.x, input.Position.y, input.Position.z, 1.0);

	verCol = int(input.Color.g * 255.9);
	if (verCol != 0)
		matTran += JointTransform[verCol - 1];

	verCol = int(input.Color.b * 255.9);
	if (verCol != 0)
		matTran += JointTransform[verCol - 1];

	verCol = int(input.Color.a * 255.9);
	if (verCol != 0)
		matTran += JointTransform[verCol - 1];

	float4x4 ModelTransform = mWorldViewProj;
	ModelTransform = mul(matTran, ModelTransform);
	float4 hpos = mul(MeshPos, ModelTransform);

	OUT.Position = hpos;

	#if defined(CP3D_COMPUTE_DEPTH_MATERIAL)
	OUT.ClipPos = computeDepthVertex(hpos);

	#elif defined(CP3D_COMPUTE_SHADOWS_MATERIAL)
	VS_OUTPUT_SHADOWS_MATERIAL outShadows = computeShadowsVertex(hpos, MeshPos.xyz, input.Normal);
	OUT.ClipPos = outShadows.ShadowMapSamplingPos;
	OUT.TexCoords2 = outShadows.MVar;
	#endif

	return OUT;
}

#endif
