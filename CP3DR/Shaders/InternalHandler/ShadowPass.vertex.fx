##ifdef DIRECT3D_11
float4x4 mWorldTrans;
##endif
float4x4 mWorldViewProj;
float4x4 mWorldViewProj2;
float3 LightPos;
float MaxD;
float MAPRES;

struct VS_OUTPUT
{
	float4 Position				: POSITION;
	float4 ShadowMapSamplingPos : TEXCOORD0; 
	float4 MVar        			: TEXCOORD1;
};

struct VS_INPUT
{
	float3 Position	: POSITION;
	float3 Normal : NORMAL;
};

VS_OUTPUT vertexMain(VS_INPUT In)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	OUT.Position = mul(float4(In.Position.x, In.Position.y, In.Position.z, 1.0), mWorldViewProj);
	float4 SMPos = mul(float4(In.Position.x, In.Position.y, In.Position.z, 1.0), mWorldViewProj2);
	SMPos.xy = float2(SMPos.x, -SMPos.y) / 2.0;
	
	OUT.ShadowMapSamplingPos = SMPos;
	
##ifdef DIRECT3D_11
	float4 worldpos = mul(float4(In.Position.x, In.Position.y, In.Position.z, 1.0), mWorldTrans);
	float3 LightDir = normalize(LightPos - worldpos.xyz);
##else
	float3 LightDir = normalize(LightPos - In.Position);
##endif
		
	OUT.MVar = float4(SMPos.z, dot(In.Normal, LightDir), 1.0 / MAPRES, MaxD);
	
	return (OUT);
}
