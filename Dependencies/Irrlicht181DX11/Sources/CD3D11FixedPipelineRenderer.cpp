// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_

#define _IRR_DONT_DO_MEMORY_DEBUGGING_HERE

#include "CD3D11FixedPipelineRenderer.h"
#include "IVideoDriver.h"
#include "IMaterialRendererServices.h"
#include "os.h"
#include "SLight.h"

namespace irr
{
namespace video  
{
	const char FIXED_FUNCTION_SHADER[] =
		"// Constants\n"\
		"#define MAX_LIGHTS 								8\n"\
		"\n"\
		"#define FOGMODE_NONE    							0\n"\
		"#define FOGMODE_LINEAR  							1\n"\
		"#define FOGMODE_EXP     							2\n"\
		"#define FOGMODE_EXP2    							3\n"\
		"#define E 										2.71828\n"\
		"\n"\
		"// Material types\n"\
		"#define EMT_SOLID									0\n"\
		"#define EMT_SOLID_2_LAYER							1\n"\
		"#define EMT_LIGHTMAP								2\n"\
		"#define EMT_LIGHTMAP_ADD							3\n"\
		"#define EMT_LIGHTMAP_M2							4\n"\
		"#define EMT_LIGHTMAP_M4							5\n"\
		"#define EMT_LIGHTMAP_LIGHTING						6\n"\
		"#define EMT_LIGHTMAP_LIGHTINM2						7\n"\
		"#define EMT_LIGHTMAP_LIGHTINM4						8\n"\
		"#define EMT_DETAIL_MAP								9\n"\
		"#define EMT_SPHERE_MAP								10\n"\
		"#define EMT_REFLECTION_2_LAYER						11\n"\
		"#define EMT_TRANSPARENT_ADD_COLOR					12\n"\
		"#define EMT_TRANSPARENT_ALPHA_CHANNEL				13\n"\
		"#define EMT_TRANSPARENT_ALPHA_CHANNEL_REF			14\n"\
		"#define EMT_TRANSPARENT_VERTEX_ALPHA				15\n"\
		"#define EMT_TRANSPARENT_REFLECTION_2_LAYER			16\n"\
		"\n"\
		"// Normal mapping\n"\
		"#define EMT_NORMAL_MAP_SOLID						17\n"\
		"#define EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR		18\n"\
		"#define EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA	19\n"\
		"\n"\
		"// Parllax mapping\n"\
		"#define EMT_PARALLAX_MAP_SOLID						20\n"\
		"#define EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR		21\n"\
		"#define EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA	22\n"\
		"\n"\
		"#define EMT_ONETEXTURE_BLEND						23\n"\
		"\n"\
		"// adding needed structs\n"\
		"struct ColorsOutput\n"\
		"{\n"\
		"	float4 diffuse;\n"\
		"	float4 specular;\n"\
		"};\n"\
		"\n"\
		"struct Light\n"\
		"{\n"\
		"	float4 position;\n"\
		"	float4 diffuse;\n"\
		"	float4 specular;\n"\
		"	float4 ambient;\n"\
		"	float4 atten;\n"\
		"};\n"\
		"\n"\
		"struct Material\n"\
		"{\n"\
		"	float4 ambient;\n"\
		"	float4 diffuse;\n"\
		"	float4 specular;\n"\
		"	float4 emissive;\n"\
		"	float shininess;\n"\
		"	int type;\n"\
		"};\n"\
		"\n"\
		"struct VS_INPUT\n"\
		"{\n"\
		"	float4 pos		: POSITION;\n"\
		"	float3 norm		: NORMAL;\n"\
		"	float4 color	: COLOR;\n"\
		"	float2 tex0		: TEXCOORD0;\n"\
		"};\n"\
		"\n"\
		"struct VS_INPUT_2COORDS\n"\
		"{\n"\
		"	float4 pos		: POSITION;\n"\
		"	float3 norm		: NORMAL;\n"\
		"	float4 color	: COLOR;\n"\
		"	float2 tex0		: TEXCOORD0;\n"\
		"	float2 tex1		: TEXCOORD1;\n"\
		"};\n"\
		"\n"\
		"struct VS_INPUT_TANGENTS\n"\
		"{\n"\
		"	float4 pos		: POSITION;\n"\
		"	float3 norm		: NORMAL;\n"\
		"	float4 color	: COLOR;\n"\
		"	float2 tex0		: TEXCOORD0;\n"\
		"	float3 tangent	: TANGENT;\n"\
		"	float3 binormal : BINORMAL;\n"\
		"};\n"\
		"\n"\
		"struct VS_OUTPUT\n"\
		"{\n"\
		"	float4 pos		: SV_Position;\n"\
		"	float2 tex0		: TEXTURE0;\n"\
		"	float3 wPos		: TEXTURE1;         //world space pos\n"\
		"	float3 wNorm	: TEXTURE2;        //world space normal\n"\
		"	float4 colorD	: COLOR0;\n"\
		"	float4 colorS	: COLOR1;\n"\
		"	float fogDist	: FOGDISTANCE;\n"\
		"	float3 planeDist: SV_ClipDistance0;\n"\
		"};\n"\
		"\n"\
		"struct VS_OUTPUT_2COORDS\n"\
		"{\n"\
		"	float4 pos		: SV_Position;\n"\
		"	float2 tex0		: TEXTURE0;\n"\
		"	float2 tex1		: TEXTURE1;\n"\
		"	float3 wPos		: TEXTURE2;         //world space pos\n"\
		"	float3 wNorm	: TEXTURE3;        //world space normal\n"\
		"	float4 colorD	: COLOR0;\n"\
		"	float4 colorS	: COLOR1;\n"\
		"	float fogDist	: FOGDISTANCE;\n"\
		"	float3 planeDist: SV_ClipDistance0;\n"\
		"};\n"\
		"\n"\
		"struct VS_OUTPUT_TANGENTS\n"\
		"{\n"\
		"	float4 pos		: SV_Position;\n"\
		"	float2 tex0		: TEXTURE0;\n"\
		"	float3 tangent	: TANGENT0;\n"\
		"	float3 binormal : BINORMAL0;\n"\
		"	float3 wPos		: TEXTURE1;         //world space pos\n"\
		"	float3 wNorm	: TEXTURE2;        //world space normal\n"\
		"	float4 colorD	: COLOR0;\n"\
		"	float4 colorS	: COLOR1;\n"\
		"	float fogDist	: FOGDISTANCE;\n"\
		"	float3 planeDist: SV_ClipDistance0;\n"\
		"};\n"\
		"\n"\
		"struct PS_INPUT\n"\
		"{\n"\
		"	float4 pos		: SV_Position;\n"\
		"	float2 tex0		: TEXTURE0;\n"\
		"	float3 wPos		: TEXTURE1;         //world space pos\n"\
		"	float3 wNorm	: TEXTURE2;        //world space normal\n"\
		"	float4 colorD	: COLOR0;\n"\
		"	float4 colorS	: COLOR1;\n"\
		"	float fogDist 	: FOGDISTANCE;\n"\
		"};\n"\
		"\n"\
		"struct PS_INPUT_2COORDS\n"\
		"{\n"\
		"	float4 pos		: SV_Position;\n"\
		"	float2 tex0		: TEXTURE0;\n"\
		"	float2 tex1		: TEXTURE1;\n"\
		"	float3 wPos		: TEXTURE2;         //world space pos\n"\
		"	float3 wNorm	: TEXTURE3;        //world space normal\n"\
		"	float4 colorD	: COLOR0;\n"\
		"	float4 colorS	: COLOR1;\n"\
		"	float fogDist 	: FOGDISTANCE;\n"\
		"};\n"\
		"\n"\
		"struct PS_INPUT_TANGENTS\n"\
		"{\n"\
		"	float4 pos		: SV_Position;\n"\
		"	float2 tex0		: TEXTURE0;\n"\
		"	float3 tangent	: TANGENT0;\n"\
		"	float3 binormal	: BINORMAL0;\n"\
		"	float3 wPos		: TEXTURE1;			//world space pos\n"\
		"	float3 wNorm	: TEXTURE2;			//world space normal\n"\
		"	float4 colorD	: COLOR0;\n"\
		"	float4 colorS	: COLOR1;\n"\
		"	float fogDist	: FOGDISTANCE;\n"\
		"};\n"\
		"\n"\
		"// adding constant buffer for transform matrices\n"\
		"cbuffer cbPerFrame : register(c0)\n"\
		"{\n"\
		"	float4x4 mWorld;\n"\
		"	float4x4 mView;\n"\
		"	float4x4 mProj;\n"\
		"	float4x4 mTexture;\n"\
		"};\n"\
		"\n"\
		"// adding constant buffer for fog, clip planes and point draw\n"\
		"cbuffer cbPerTechnique : register(c1)\n"\
		"{\n"\
		"   float4 fogColor;\n"\
		"	int fogMode = FOGMODE_NONE;\n"\
		"   float fogStart;\n"\
		"   float fogEnd;\n"\
		"   float fogDensity;\n"\
		"\n"\
		"	bool enableAlpha = false;\n"\
		"\n"\
		"   bool enableLighting = false;\n"\
		"\n"\
		"   bool enableClipping = false;\n"\
		"\n"\
		"	bool enableFog = false;\n"\
		"\n"\
		"   bool enablePointScale = false;\n"\
		"   float pointScaleA;\n"\
		"   float pointScaleB;\n"\
		"   float pointScaleC;\n"\
		"   float pointSize;\n"\
		"};\n"\
		"\n"\
		"// adding constant buffer for lightning information\n"\
		"cbuffer cbLights : register(c2)\n"\
		"{\n"\
		"	float4 clipPlanes[3];\n"\
		"	Light lights[MAX_LIGHTS];\n"\
		"	Material material;\n"\
		"	int lightCount = 0; // Number of lights enabled\n"\
		"};\n"\
		"\n"\
		"// adding function to calculate lightning\n"\
		"ColorsOutput calcLighting( float3 worldNormal, float3 worldPos, float3 cameraPos, float4 vertexColour)\n"\
		"{\n"\
		"	ColorsOutput output = (ColorsOutput)0;\n"\
		"\n"\
		"	const int nLights = min(MAX_LIGHTS, lightCount); // Clamp to MAX_LIGHTS\n"\
		"\n"\
		"	for(int i=0; i<nLights; ++i)\n"\
		"	{\n"\
		"		float3 toLight = lights[i].position.xyz - worldPos;\n"\
		"		float lightDist = length( toLight );\n"\
		"		float fAtten = 1.0/dot( lights[i].atten, float4(1,lightDist,lightDist*lightDist,0) );\n"\
		"		float3 lightDir = normalize( toLight );\n"\
		"		float3 halfAngle = normalize( normalize(-cameraPos) + lightDir );\n"\
		"\n"\
		"		float4 _Ambient = lights[i].ambient * material.ambient;\n"\
		"		float4 _Diffuse = lights[i].diffuse * material.diffuse * vertexColour;\n"\
		"		float4 _Specular = lights[i].specular * material.specular;\n"\
		"		float4 _Emissive = material.emissive;\n"\
		"\n"\
		"		output.diffuse += max(0, dot( lightDir, worldNormal ) * _Diffuse * fAtten) + _Ambient + _Emissive;\n"\
		"		output.specular += max(0, pow( abs( dot( halfAngle, worldNormal ) ), 64 ) * _Specular * fAtten );\n"\
		"	}\n"\
		"\n"\
		"	return output;\n"\
		"}\n"\
		"\n"\
		"// adding function to calculate fog\n"\
		"float calcFogFactor( float d )\n"\
		"{\n"\
		"	float fogCoeff = 1.0;\n"\
		"\n"\
		"	if(!enableFog)\n"\
		"		return fogCoeff;\n"\
		"	else\n"\
		"	{\n"\
		"		[branch]\n"\
		"		switch( fogMode )\n"\
		"		{\n"\
		"		case FOGMODE_LINEAR:\n"\
		"			fogCoeff = (fogEnd - d)/(fogEnd - fogStart);\n"\
		"			break;\n"\
		"		case FOGMODE_EXP:\n"\
		"			fogCoeff = 1.0 / pow( E, d*fogDensity );\n"\
		"			break;\n"\
		"		case FOGMODE_EXP2:\n"\
		"			fogCoeff = 1.0 / pow( E, d*d*fogDensity*fogDensity );\n"\
		"			break;\n"\
		"		};\n"\
		"\n"\
		"		return clamp( fogCoeff, 0, 1 );\n"\
		"	}\n"\
		"}\n"\
		"\n"\
		"// adding vertex shader code\n"\
		"VS_OUTPUT standardVS( VS_INPUT input )\n"\
		"{\n"\
		"	VS_OUTPUT output = (VS_OUTPUT)0;\n"\
		"\n"\
		"	//output our final position in clipspace\n"\
		"	float4 worldPos = mul( input.pos, mWorld );\n"\
		"	float4 cameraPos = mul( worldPos, mView ); //Save cameraPos for fog calculations\n"\
		"	output.pos = mul( cameraPos, mProj );\n"\
		"\n"\
		"	output.wPos = worldPos.xyz;\n"\
		"\n"\
		"	//save the fog distance for later\n"\
		"	output.fogDist = distance(output.pos, cameraPos);\n"\
		"\n"\
		"   //find our clipping planes (fixed function clipping is done in world space)\n"\
		"	[branch]\n"\
		"	if( enableClipping )\n"\
		"   {\n"\
		"		worldPos.w = 1;\n"\
		"\n"\
		"		//calc the distance from the 3 clipping planes\n"\
		"       output.planeDist.x = dot( worldPos, clipPlanes[0] );\n"\
		"       output.planeDist.y = dot( worldPos, clipPlanes[1] );\n"\
		"       output.planeDist.z = dot( worldPos, clipPlanes[2] );\n"\
		"   }\n"\
		"	else\n"\
		"   {\n"\
		"       output.planeDist.x = 1;\n"\
		"       output.planeDist.y = 1;\n"\
		"       output.planeDist.z = 1;\n"\
		"	}\n"\
		"\n"\
		"	float4 diffuseSwizzle = input.color.bgra; // swizzle the input rgba channels into D3D11 order\n"\
		"\n"\
		"	[branch]\n"\
		"	if( enableLighting )\n"\
		"	{\n"\
		"		float3 worldNormal = normalize( mul( input.norm, (float3x3)mWorld ) );\n"\
		"		output.wNorm = worldNormal;\n"\
		"		ColorsOutput cOut = calcLighting( worldNormal, worldPos.xyz, cameraPos.xyz, diffuseSwizzle );\n"\
		"		output.colorD = cOut.diffuse;\n"\
		"		output.colorS = cOut.specular;\n"\
		"	}\n"\
		"	else\n"\
		"	{\n"\
		"		output.colorD = diffuseSwizzle;\n"\
		"	}\n"\
		"\n"\
		"	// propagate texture coordinate\n"\
		"	output.tex0 = input.tex0;\n"\
		"	return output;\n"\
		"}\n"\
		"\n"\
		"// adding vertex shader code\n"\
		"VS_OUTPUT_2COORDS coords2TVS( VS_INPUT_2COORDS input )\n"\
		"{\n"\
		"	VS_OUTPUT_2COORDS output = (VS_OUTPUT_2COORDS)0;\n"\
		"\n"\
		"	//output our final position in clipspace\n"\
		"	float4 worldPos = mul( input.pos, mWorld );\n"\
		"	float4 cameraPos = mul( worldPos, mView ); //Save cameraPos for fog calculations\n"\
		"	output.pos = mul( cameraPos, mProj );\n"\
		"\n"\
		"	output.wPos = worldPos.xyz;\n"\
		"\n"\
		"	//save the fog distance for later\n"\
		"	output.fogDist = distance(output.pos, cameraPos);\n"\
		"\n"\
		"   //find our clipping planes (fixed function clipping is done in world space)\n"\
		"	[branch]\n"\
		"   if( enableClipping )\n"\
		"   {\n"\
		"       worldPos.w = 1;\n"\
		"\n"\
		"       //calc the distance from the 3 clipping planes\n"\
		"       output.planeDist.x = dot( worldPos, clipPlanes[0] );\n"\
		"       output.planeDist.y = dot( worldPos, clipPlanes[1] );\n"\
		"       output.planeDist.z = dot( worldPos, clipPlanes[2] );\n"\
		"   }\n"\
		"	else\n"\
		"   {\n"\
		"       output.planeDist.x = 1;\n"\
		"       output.planeDist.y = 1;\n"\
		"       output.planeDist.z = 1;\n"\
		"   }\n"\
		"\n"\
		"	float4 diffuseSwizzle = input.color.bgra; // swizzle the input rgba channels into D3D10 order\n"\
		"\n"\
		"	[branch]\n"\
		"	if( enableLighting )\n"\
		"	{\n"\
		"		float3 worldNormal = normalize( mul( input.norm, (float3x3)mWorld ) );\n"\
		"		output.wNorm = worldNormal;\n"\
		"		ColorsOutput cOut = calcLighting( worldNormal, worldPos.xyz, cameraPos.xyz, diffuseSwizzle );\n"\
		"		output.colorD = cOut.diffuse;\n"\
		"		output.colorS = cOut.specular;\n"\
		"	}\n"\
		"	else\n"\
		"	{\n"\
		"		output.colorD = diffuseSwizzle;\n"\
		"	}\n"\
		"\n"\
		"	// propagate texture coordinate\n"\
		"	output.tex0 = input.tex0;\n"\
		"	output.tex1 = input.tex1;\n"\
		"	return output;\n"\
		"}\n"\
		"\n"\
		"// adding vertex shader code\n"\
		"VS_OUTPUT_TANGENTS tangentsVS( VS_INPUT_TANGENTS input )\n"\
		"{\n"\
		"	VS_OUTPUT_TANGENTS output = (VS_OUTPUT_TANGENTS)0;\n"\
		"\n"\
		"	//output our final position in clipspace\n"\
		"	float4 worldPos = mul( input.pos, mWorld );\n"\
		"	float4 cameraPos = mul( worldPos, mView ); //Save cameraPos for fog calculations\n"\
		"	output.pos = mul( cameraPos, mProj );\n"\
		"\n"\
		"	output.wPos = worldPos.xyz;\n"\
		"\n"\
		"	//save the fog distance for later\n"\
		"	output.fogDist = distance(output.pos, cameraPos);\n"\
		"\n"\
		"   //find our clipping planes (fixed function clipping is done in world space)\n"\
		"	[branch]\n"\
		"   if( enableClipping )\n"\
		"   {\n"\
		"       worldPos.w = 1;\n"\
		"\n"\
		"       //calc the distance from the 3 clipping planes\n"\
		"       output.planeDist.x = dot( worldPos, clipPlanes[0] );\n"\
		"       output.planeDist.y = dot( worldPos, clipPlanes[1] );\n"\
		"       output.planeDist.z = dot( worldPos, clipPlanes[2] );\n"\
		"   }\n"\
		"	else\n"\
		"   {\n"\
		"       output.planeDist.x = 1;\n"\
		"       output.planeDist.y = 1;\n"\
		"       output.planeDist.z = 1;\n"\
		"   }\n"\
		"\n"\
		"	float4 diffuseSwizzle = input.color.bgra; // swizzle the input rgba channels into D3D10 order\n"\
		"\n"\
		"	[branch]\n"\
		"	if( enableLighting )\n"\
		"	{\n"\
		"		float3 worldNormal = normalize( mul( input.norm, (float3x3)mWorld ) );\n"\
		"		output.wNorm = worldNormal;\n"\
		"		ColorsOutput cOut = calcLighting( worldNormal, worldPos.xyz, cameraPos.xyz, diffuseSwizzle );\n"\
		"		output.colorD = cOut.diffuse;\n"\
		"		output.colorS = cOut.specular;\n"\
		"	}\n"\
		"	else\n"\
		"	{ \n"\
		"		output.colorD = diffuseSwizzle;\n"\
		"	}\n"\
		"\n"\
		"	// propagate texture coordinate\n"\
		"	output.tex0 = input.tex0;\n"\
		"	output.tangent = input.tangent;\n"\
		"	output.binormal = input.binormal;\n"\
		"	return output;\n"\
		"}\n"\
		"\n"\
		"// adding textures and samplers\n"\
		"Texture2D tex1 : register(t0);\n"\
		"Texture2D tex2 : register(t1);\n"\
		"\n"\
		"SamplerState sampler1 : register(s0);\n"\
		"SamplerState sampler2 : register(s1);\n"\
		"\n"\
		"// adding pixel shader\n"\
		"float4 standardPS( PS_INPUT input ) : SV_Target\n"\
		"{\n"\
		"	float4 normalColor = float4(0, 0, 0, 1);\n"\
		"	float4 tex1C = tex1.Sample( sampler1, input.tex0 ).bgra;\n"\
		"	float4 tex2C = tex2.Sample( sampler2, input.tex0 ).bgra;\n"\
		"\n"\
		"	// check material type, and add color operation correctly\n"\
		"	[branch]\n"\
		"	switch(material.type)\n"\
		"	{\n"\
		"	case EMT_SOLID:\n"\
		"		normalColor = (tex1C * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"   case EMT_SOLID_2_LAYER:\n"\
		"		normalColor = lerp(tex1C, tex2C, tex1C.a) * input.colorD + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP:\n"\
		"		normalColor = tex1C * tex2C;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_ADD:\n"\
		"		normalColor = tex1C + tex2C;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_M2:\n"\
		"		normalColor = (tex1C * tex2C) * 2;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_M4:\n"\
		"		normalColor = (tex1C * tex2C) * 4;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_LIGHTING:\n"\
		"		normalColor = tex1C * tex2C;\n"\
		"		normalColor = (normalColor * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_LIGHTINM2:\n"\
		"		normalColor = (tex1C * tex2C) * 2;\n"\
		"		normalColor = (normalColor * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_LIGHTINM4:\n"\
		"		normalColor = (tex1C * tex2C) * 4;\n"\
		"		normalColor = (normalColor * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_DETAIL_MAP:\n"\
		"		normalColor = (tex1C + tex2C) - 0.5;\n"\
		"		break;\n"\
		"	case EMT_SPHERE_MAP:							// TODO\n"\
		"		normalColor = tex1C;//tex1C * normalize(reflect(-normalize(input.pos - input.wPos), normalize(input.wNorm)));\n"\
		"		//normalColor = (tex1C * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_REFLECTION_2_LAYER:					// TODO\n"\
		"		normalColor = (tex1C * tex2C);\n"\
		"		normalColor = (normalColor * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_TRANSPARENT_ADD_COLOR:					// TODO\n"\
		"		normalColor = (tex1C * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_TRANSPARENT_VERTEX_ALPHA:\n"\
		"		//normalColor = tex1C + input.colorS;\n"\
		"		normalColor = float4(tex1C.bgr,input.colorS.a );\n"\
		"		//normalColor = (float4(tex1C.rgb, input.colorD.a) /** input.colorD*/) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_TRANSPARENT_ALPHA_CHANNEL:\n"\
		"		normalColor =  tex1C * input.colorD + input.colorS;\n"\
		"		normalColor.a =  tex1C.a;\n"\
		"		break;\n"\
		"	case EMT_TRANSPARENT_ALPHA_CHANNEL_REF:\n"\
		"		normalColor = 0;\n"\
		"		clip(-1);\n"\
		"		break;\n"\
		"	case EMT_TRANSPARENT_REFLECTION_2_LAYER:		// TODO\n"\
		"		normalColor = tex1C + input.colorS;;\n"\
		"		break;\n"\
		"	case EMT_ONETEXTURE_BLEND:						// TODO\n"\
		"		normalColor = tex1C /* input.colorD*/ + input.colorS;\n"\
		"		break;\n"\
		"	default:\n"\
		"		normalColor = tex1C;\n"\
		"		break;\n"\
		"	};\n"\
		"\n"\
		"	// fog calculation\n"\
		"	float fog = calcFogFactor( input.fogDist );\n"\
		"	normalColor = fog * normalColor + (1.0 - fog)*fogColor;\n"\
		"\n"\
		"	// return color in pixel shader\n"\
		"	return normalColor;\n"\
		"}\n"\
		"\n"\
		"float4 coords2TPS( PS_INPUT_2COORDS input ) : SV_Target\n"\
		"{\n"\
		"	float4 normalColor = float4(0, 0, 0, 1);\n"\
		"	float4 tex1C = tex1.Sample( sampler1, input.tex0 ).bgra;\n"\
		"	float4 tex2C = tex2.Sample( sampler2, input.tex1 ).bgra;\n"\
		"\n"\
		"	// check material type, and add color operation correctly\n"\
		"	[branch]\n"\
		"	switch(material.type)\n"\
		"	{\n"\
		"	case EMT_SOLID:\n"\
		"		normalColor = (tex1C * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_SOLID_2_LAYER:\n"\
		"		normalColor = lerp(tex1C, tex2C, tex1C.a) * input.colorD + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP:\n"\
		"		normalColor = tex1C * tex2C;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_ADD:\n"\
		"		normalColor = tex1C + tex2C;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_M2:\n"\
		"		normalColor = (tex1C * tex2C) * 2;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_M4:\n"\
		"		normalColor = (tex1C * tex2C) * 4;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_LIGHTING:\n"\
		"		normalColor = tex1C * tex2C;\n"\
		"		normalColor = (normalColor * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_LIGHTINM2:\n"\
		"		normalColor = (tex1C * tex2C) * 2;\n"\
		"		normalColor = (normalColor * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_LIGHTMAP_LIGHTINM4:\n"\
		"		normalColor = (tex1C * tex2C) * 4;\n"\
		"		normalColor = (normalColor * input.colorD) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_DETAIL_MAP:\n"\
		"		normalColor = (tex1C + tex2C) - 0.5;\n"\
		"		break;\n"\
		"	case EMT_SPHERE_MAP:							// TODO\n"\
		"	case EMT_REFLECTION_2_LAYER:					// TODO\n"\
		"	case EMT_TRANSPARENT_ADD_COLOR:					// TODO\n"\
		"		normalColor = (tex1C /** input.colorD*/) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_TRANSPARENT_ALPHA_CHANNEL:				// TODO\n"\
		"	case EMT_TRANSPARENT_ALPHA_CHANNEL_REF:\n"\
		"		normalColor = 0;\n"\
		"		clip(-1);\n"\
		"		break;\n"\
		"	case EMT_TRANSPARENT_VERTEX_ALPHA:\n"\
		"		normalColor = (tex1C /** input.colorD*/) + input.colorS;\n"\
		"		//normalColor = (float4(tex1C.rgb, input.colorD.a) /** input.colorD*/) + input.colorS;\n"\
		"		break;\n"\
		"	case EMT_TRANSPARENT_REFLECTION_2_LAYER:		// TODO\n"\
		"	case EMT_ONETEXTURE_BLEND:						// TODO\n"\
		"	default:\n"\
		"		normalColor = tex1C;\n"\
		"		break;\n"\
		"	}\n"\
		"\n"\
		"	// fog calculation\n"\
		"	float fog = calcFogFactor( input.fogDist );\n"\
		"	normalColor = fog * normalColor + (1.0 - fog)*fogColor;\n"\
		"\n"\
		"	// return color in pixel shader\n"\
		"	return normalColor;\n"\
		"}\n"\
		"\n"\
		"float4 tangentsPS( PS_INPUT_TANGENTS input ) : SV_Target\n"\
		"{\n"\
		"	float4 normalColor = float4(0, 0, 0, 1);\n"\
		"	float4 tex1C = tex1.Sample( sampler1, input.tex0 ).bgra;\n"\
		"\n"\
		"	// Only use EMT_SOLID for now\n"\
		"	normalColor = (tex1C * input.colorD) + input.colorS;\n"\
		"\n"\
		"	// fog calculation\n"\
		"	float fog = calcFogFactor( input.fogDist );\n"\
		"	normalColor = fog * normalColor + (1.0 - fog)*fogColor;\n"\
		"\n"\
		"	// return color in pixel shader\n"\
		"	return normalColor;\n"\
		"}\n";

CD3D11FixedPipelineRenderer::CD3D11FixedPipelineRenderer( ID3D11Device* device, IVideoDriver* driver, CD3D11CallBridge* bridgeCalls) 
	: CD3D11MaterialRenderer(device, driver, bridgeCalls, NULL, NULL),
	cbPerFrameId(-1), cbPerTechniqueId(-1), cbLightsId(-1),
	vsStandardShader(NULL), vsCoords2TShader(NULL), vsTangentsShader(NULL),
	psStandardShader(NULL), psCoords2TShader(NULL), psTangentsShader(NULL),
	standardBuffer(NULL), tangentsBuffer(NULL), coords2TBuffer(NULL)
{
#ifdef _DEBUG
	setDebugName("CD3D11FixedPipelineRenderer");
#endif
	CallBack = this;

	IMaterialRenderer* solidRend = Driver->getMaterialRenderer(EMT_SOLID);

	if (solidRend)
	{
		CD3D11FixedPipelineRenderer* r = static_cast<CD3D11FixedPipelineRenderer*>(solidRend);

		vsStandardShader = r->vsStandardShader;

		if(vsStandardShader)
			vsStandardShader->AddRef();

		psStandardShader = r->psStandardShader;

		if(psStandardShader)
			psStandardShader->AddRef();

		vsTangentsShader = r->vsTangentsShader;

		if(vsTangentsShader)
			vsTangentsShader->AddRef();

		psTangentsShader = r->psTangentsShader;

		if(psTangentsShader)
			psTangentsShader->AddRef();

		vsCoords2TShader = r->vsCoords2TShader;

		if(vsCoords2TShader)
			vsCoords2TShader->AddRef();

		psCoords2TShader = r->psCoords2TShader;

		if(psCoords2TShader)
			psCoords2TShader->AddRef();

		standardBuffer = r->standardBuffer;

		if(standardBuffer)
			standardBuffer->AddRef();

		tangentsBuffer = r->tangentsBuffer;

		if(tangentsBuffer)
			tangentsBuffer->AddRef();

		coords2TBuffer = r->coords2TBuffer;

		if(coords2TBuffer)
			coords2TBuffer->AddRef();

		cbPerFrameId = r->cbPerFrameId;
		cbLightsId = r->cbLightsId;
		cbPerTechniqueId = r->cbPerTechniqueId;
	}
	else
	{
		if(driver->queryFeature(EVDF_VERTEX_SHADER_5_0))
		{	
			if(!init(FIXED_FUNCTION_SHADER, "standardVS", EVST_VS_5_0,
				FIXED_FUNCTION_SHADER, "standardPS", EPST_PS_5_0))
				return;

			vsStandardShader = shaders[EST_VERTEX_SHADER];
			psStandardShader = shaders[EST_PIXEL_SHADER];
			standardBuffer = buffer;

			if(!init(FIXED_FUNCTION_SHADER, "coords2TVS", EVST_VS_5_0,
				FIXED_FUNCTION_SHADER, "coords2TPS", EPST_PS_5_0))
				return;

			vsCoords2TShader = shaders[EST_VERTEX_SHADER];
			psCoords2TShader = shaders[EST_PIXEL_SHADER];
			coords2TBuffer = buffer;

			if(!init(FIXED_FUNCTION_SHADER, "tangentsVS", EVST_VS_5_0,
				FIXED_FUNCTION_SHADER, "tangentsPS", EPST_PS_5_0))
				return;

			vsTangentsShader = shaders[EST_VERTEX_SHADER];
			psTangentsShader = shaders[EST_PIXEL_SHADER];
			tangentsBuffer = buffer;
		}
		else
		{
			if(!init(FIXED_FUNCTION_SHADER, "standardVS", EVST_VS_4_1,
				FIXED_FUNCTION_SHADER, "standardPS", EPST_PS_4_1))
				return;

			vsStandardShader = shaders[EST_VERTEX_SHADER];
			psStandardShader = shaders[EST_PIXEL_SHADER];
			standardBuffer = buffer;

			if(!init(FIXED_FUNCTION_SHADER, "coords2TVS", EVST_VS_4_1,
				FIXED_FUNCTION_SHADER, "coords2TPS", EPST_PS_4_1))
				return;

			vsCoords2TShader = shaders[EST_VERTEX_SHADER];
			psCoords2TShader = shaders[EST_PIXEL_SHADER];
			coords2TBuffer = buffer;
		
			if(!init(FIXED_FUNCTION_SHADER, "tangentsVS", EVST_VS_4_1,
				FIXED_FUNCTION_SHADER, "tangentsPS", EPST_PS_4_1))
				return;

			vsTangentsShader = shaders[EST_VERTEX_SHADER];
			psTangentsShader = shaders[EST_PIXEL_SHADER];
			tangentsBuffer = buffer;
		}

		cbPerFrameId = getConstantBufferID("cbPerFrame", EST_VERTEX_SHADER);
		cbPerTechniqueId = getConstantBufferID("cbPerTechnique", EST_VERTEX_SHADER);
		cbLightsId = getConstantBufferID("cbLights", EST_VERTEX_SHADER);
	}
}

CD3D11FixedPipelineRenderer::~CD3D11FixedPipelineRenderer()
{
	if(CallBack == this)
		CallBack = NULL;

	shaders[EST_VERTEX_SHADER] = NULL;
	shaders[EST_PIXEL_SHADER] = NULL;
	buffer = NULL;

	if(vsStandardShader)
		vsStandardShader->Release();

	if(psStandardShader)
		psStandardShader->Release();

	if(vsTangentsShader)
		vsTangentsShader->Release();

	if(psTangentsShader)
		psTangentsShader->Release();

	if(vsCoords2TShader)
		vsCoords2TShader->Release();

	if(psCoords2TShader)
		psCoords2TShader->Release();

	if(standardBuffer)
		standardBuffer->Release();

	if(tangentsBuffer)
		tangentsBuffer->Release();

	if(coords2TBuffer)
		coords2TBuffer->Release();
}

bool CD3D11FixedPipelineRenderer::OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype)
{
	switch (vtxtype)
	{
	case EVT_STANDARD:
		shaders[EST_VERTEX_SHADER] = vsStandardShader;
		shaders[EST_PIXEL_SHADER] = psStandardShader;
		buffer = standardBuffer;
		break;
	case EVT_2TCOORDS:
		shaders[EST_VERTEX_SHADER] = vsCoords2TShader;
		shaders[EST_PIXEL_SHADER] = psCoords2TShader;
		buffer = coords2TBuffer;
		break;
	case EVT_TANGENTS:
		shaders[EST_VERTEX_SHADER] = vsTangentsShader;
		shaders[EST_PIXEL_SHADER] = psTangentsShader;
		buffer = tangentsBuffer;
		break;
	default:
		shaders[EST_VERTEX_SHADER] = NULL;
		shaders[EST_PIXEL_SHADER] = NULL;
		os::Printer::log("Error: vertextype is not supported by FixedPipelineRenderer", ELL_ERROR);
		break;
	}

	return CD3D11MaterialRenderer::OnRender(service, vtxtype);
}

void CD3D11FixedPipelineRenderer::OnSetConstants( IMaterialRendererServices* service, s32 userData )
{
	// set fog
	SColor fogColor;
	video::E_FOG_TYPE fogMode;
	float fogStart = 0.0f;
	float fogEnd = 0.0f;
	float fogDensity = 0.0f;
	bool pixelFog = true;
	bool rangeFog = true;

	Driver->getFog( fogColor, fogMode, fogStart, fogEnd, fogDensity, pixelFog, rangeFog );

	cbPerTechnique.enableFog = CurrentMaterial.FogEnable;
	cbPerTechnique.fogColor = fogColor;
	cbPerTechnique.fogMode = fogMode;
	cbPerTechnique.fogStart = fogStart;
	cbPerTechnique.fogEnd = fogEnd;
	cbPerTechnique.fogDensity = fogDensity;

	// clipping
	cbPerTechnique.enableClipping = true;

	// set material
	SShaderMaterial mtl;
	ZeroMemory(&mtl, sizeof(SShaderMaterial));
	mtl.ambient = CurrentMaterial.AmbientColor;
	mtl.diffuse = CurrentMaterial.DiffuseColor;
	mtl.specular = CurrentMaterial.SpecularColor;
	mtl.emissive = CurrentMaterial.EmissiveColor;
	mtl.type = CurrentMaterial.MaterialType;
	mtl.shininess = CurrentMaterial.Shininess;

	cbLights.material = mtl;

	// set lightning
	cbPerTechnique.enableLighting = CurrentMaterial.Lighting;

	if(CurrentMaterial.Lighting)
	{
		const u32 count = Driver->getDynamicLightCount();

		cbLights.lightCount = count;

		if(count)
		{
			for(u32 i = 0; i < count && i < 8; ++i)
			{
				SShaderLight l;
				SLight dl = Driver->getDynamicLight(i);

				dl.Position.getAs4Values(&l.position.r);
				l.ambient = dl.AmbientColor;
				l.diffuse = dl.DiffuseColor;
				l.specular = dl.SpecularColor;
				dl.Attenuation.getAs4Values(&l.atten.r);

				cbLights.lights[i] = l;
			}
		}
	}

	// apply transformations
	core::matrix4 mat = Driver->getTransform( video::ETS_PROJECTION );
	cbPerFrame.mProj = mat.getTransposed();

	mat = Driver->getTransform( video::ETS_VIEW );
	cbPerFrame.mView = mat.getTransposed();

	mat = Driver->getTransform( video::ETS_WORLD );
	cbPerFrame.mWorld = mat.getTransposed();

	setConstantBuffer(cbPerFrameId, &cbPerFrame, EST_VERTEX_SHADER);
	setConstantBuffer(cbPerTechniqueId, &cbPerTechnique, EST_VERTEX_SHADER);
	setConstantBuffer(cbLightsId, &cbLights, EST_VERTEX_SHADER);
}

void CD3D11FixedPipelineRenderer::OnSetMaterial( const SMaterial& material )
{
	CurrentMaterial = material;
}

}
}

#endif