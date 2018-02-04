#ifdef OPENGL_DRIVER

struct VS_OUTPUT 
{
	vec4 ShadowMapSamplingPos;
	vec4 MVar;
};

uniform float MaxD0, MaxD1, MaxD2, MaxD3, MaxD4, MaxD5;
uniform float MAPRES0, MAPRES1, MAPRES2, MAPRES3, MAPRES4, MAPRES5;
uniform vec3 LightPos0, LightPos1, LightPos2, LightPos3, LightPos4, LightPos5;
uniform mat4 mWorldViewProj;
uniform mat4 mWorldViewProjLight0, mWorldViewProjLight1, mWorldViewProjLight2,
             mWorldViewProjLight3, mWorldViewProjLight4, mWorldViewProjLight5;

varying vec4 SMPos0, SMPos1, SMPos2, SMPos3, SMPos4, SMPos5;
varying vec4 MVar0, MVar1, MVar2, MVar3, MVar4, MVar5;

VS_OUTPUT vertexMain(in vec3 Position, in mat4 worldViewProj, in vec3 lightPos, in float MaxD, in float MAPRES)
{
	VS_OUTPUT OUT;

	OUT.ShadowMapSamplingPos = (worldViewProj * vec4(gl_Vertex.xyz, 1.0));

	vec3 lightDir = normalize(lightPos - gl_Vertex.xyz);
	
	OUT.MVar.x = OUT.ShadowMapSamplingPos.z;
	OUT.MVar.y = dot(normalize(gl_Normal.xyz), lightDir);
	OUT.MVar.z = MaxD;
	OUT.MVar.w = 1.0 / MAPRES;

	return OUT;
}

void main() 
{
    gl_Position = (mWorldViewProj * vec4(gl_Vertex.x, gl_Vertex.y, gl_Vertex.z, 1.0));

	VS_OUTPUT vOut = vertexMain(gl_Position.xyz, mWorldViewProjLight0, LightPos0, MaxD0, MAPRES0);
	SMPos0 = vOut.ShadowMapSamplingPos;
	MVar0 = vOut.MVar;

    #if (LIGHTS_COUNT >= 2)
    VS_OUTPUT vOut2 = vertexMain(gl_Position.xyz, mWorldViewProjLight1, LightPos1, MaxD1, MAPRES1);
    SMPos1 = vOut2.ShadowMapSamplingPos;
    MVar1 = vOut2.MVar;
    #endif

    #if (LIGHTS_COUNT >= 3)
    VS_OUTPUT vOut3 = vertexMain(gl_Position.xyz, mWorldViewProjLight2, LightPos2, MaxD2, MAPRES2);
    SMPos2 = vOut3.ShadowMapSamplingPos;
    MVar2 = vOut3.MVar;
    #endif

    #if (LIGHTS_COUNT >= 4)
    VS_OUTPUT vOut4 = vertexMain(gl_Position.xyz, mWorldViewProjLight3, LightPos3, MaxD3, MAPRES3);
    SMPos3 = vOut4.ShadowMapSamplingPos;
    MVar3 = vOut4.MVar;
    #endif

    #if (LIGHTS_COUNT >= 5)
    VS_OUTPUT vOut5 = vertexMain(gl_Position.xyz, mWorldViewProjLight4, LightPos4, MaxD4, MAPRES4);
    SMPos4 = vOut5.ShadowMapSamplingPos;
    MVar4 = vOut5.MVar;
    #endif

    #if (LIGHTS_COUNT >= 6)
    VS_OUTPUT vOut6 = vertexMain(gl_Position.xyz, mWorldViewProjLight5, LightPos5, MaxD5, MAPRES5);
    SMPos5 = vOut6.ShadowMapSamplingPos;
    MVar5 = vOut6.MVar;
    #endif
}

#else

#ifdef DIRECT3D_11
float4x4 mWorldTrans;
#endif
float4x4 mWorldViewProj;
float4x4 mWorldViewProjLight0, mWorldViewProjLight1, mWorldViewProjLight2, mWorldViewProjLight3, mWorldViewProjLight4, mWorldViewProjLight5;
float3 LightPos0, LightPos1, LightPos2, LightPos3, LightPos4, LightPos5;
float MaxD0, MaxD1, MaxD2, MaxD3, MaxD4, MaxD5;
float MAPRES0, MAPRES1, MAPRES2, MAPRES3, MAPRES4, MAPRES5;

struct VS_OUTPUT
{
	float4 Position				 : SV_Position;

	float4 ShadowMapSamplingPos0 : TEXCOORD0; 
	float4 MVar0        		 : TEXCOORD1;

	#if (LIGHTS_COUNT >= 2)
	float4 ShadowMapSamplingPos1 : TEXCOORD2;
	float4 MVar1        		 : TEXCOORD3;
	#endif

	#if (LIGHTS_COUNT >= 3)
	float4 ShadowMapSamplingPos2 : TEXCOORD4;
	float4 MVar2        		 : TEXCOORD5;
	#endif

	#if (LIGHTS_COUNT >= 4)
	float4 ShadowMapSamplingPos3 : TEXCOORD6;
	float4 MVar3        		 : TEXCOORD7;
	#endif
};

struct VS_INPUT
{
	float3 Position	: POSITION;
	float3 Normal : NORMAL;
};

struct LIGHT_OUTPUT
{
	float4 ShadowMapSamplingPos : TEXCOORD0;
	float4 MVar        			: TEXCOORD1;
};

LIGHT_OUTPUT getLightOutput(VS_INPUT In, float4x4 mWorldViewProjLight, float3 LightPos, float MAPRES, float MaxD)
{
	LIGHT_OUTPUT OUT = (LIGHT_OUTPUT)0;
	float4 SMPos = mul(float4(In.Position.x, In.Position.y, In.Position.z, 1.0), mWorldViewProjLight);
	SMPos.xy = float2(SMPos.x, -SMPos.y) / 2.0;

	OUT.ShadowMapSamplingPos = SMPos;

	#ifdef DIRECT3D_11
	float4 worldpos = mul(float4(In.Position.x, In.Position.y, In.Position.z, 1.0), mWorldTrans);
	float3 LightDir = normalize(LightPos - worldpos.xyz);
	#else
	float3 LightDir = normalize(LightPos - In.Position);
	#endif

	OUT.MVar = float4(SMPos.z, dot(In.Normal, LightDir), 1.0 / MAPRES, MaxD);

	return (OUT);
}

VS_OUTPUT vertexMain(VS_INPUT In)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	OUT.Position = mul(float4(In.Position.x, In.Position.y, In.Position.z, 1.0), mWorldViewProj);

	// Light 1
	LIGHT_OUTPUT output0 = getLightOutput(In, mWorldViewProjLight0, LightPos0, MAPRES0, MaxD0);
	OUT.ShadowMapSamplingPos0 = output0.ShadowMapSamplingPos;
	OUT.MVar0 = output0.MVar;

	// Light 2
	#if (LIGHTS_COUNT >= 2)
	LIGHT_OUTPUT output1 = getLightOutput(In, mWorldViewProjLight1, LightPos1, MAPRES1, MaxD1);
	OUT.ShadowMapSamplingPos1 = output1.ShadowMapSamplingPos;
	OUT.MVar1 = output1.MVar;
	#endif

	// Light 3
	#if (LIGHTS_COUNT >= 3)
	LIGHT_OUTPUT output2 = getLightOutput(In, mWorldViewProjLight2, LightPos2, MAPRES2, MaxD2);
	OUT.ShadowMapSamplingPos2 = output2.ShadowMapSamplingPos;
	OUT.MVar2 = output2.MVar;
	#endif

	// Light 3
	#if (LIGHTS_COUNT >= 4)
	LIGHT_OUTPUT output3 = getLightOutput(In, mWorldViewProjLight3, LightPos3, MAPRES3, MaxD3);
	OUT.ShadowMapSamplingPos3 = output3.ShadowMapSamplingPos;
	OUT.MVar3 = output3.MVar;
	#endif

	return (OUT);
}

#endif
