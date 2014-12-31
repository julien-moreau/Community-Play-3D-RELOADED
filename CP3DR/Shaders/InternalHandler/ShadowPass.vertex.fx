
#ifdef OPENGL_DRIVER

struct VS_OUTPUT 
{
	vec4 Position;
	vec4 ShadowMapSamplingPos;
	vec4 MVar;
};

uniform float MaxD, MAPRES;
uniform vec3 LightPos;
uniform mat4 mWorldViewProj;
uniform mat4 mWorldViewProj2;

VS_OUTPUT vertexMain( in vec3 Position) 
{
	VS_OUTPUT OUT;

	OUT.Position = (mWorldViewProj * vec4(Position.x, Position.y, Position.z, 1.0));
	OUT.ShadowMapSamplingPos = (mWorldViewProj2 * vec4(Position.x, Position.y, Position.z, 1.0));

	vec3 lightDir = normalize(LightPos - Position);
	
	OUT.MVar.x = OUT.ShadowMapSamplingPos.z;
	OUT.MVar.y = dot(normalize(gl_Normal.xyz), lightDir);
	OUT.MVar.z = MaxD;
	OUT.MVar.w = 1.0 / MAPRES;

	return OUT;
}

void main() 
{
	VS_OUTPUT vOut = vertexMain(gl_Vertex.xyz);

	gl_Position = vOut.Position;
	gl_TexCoord[0] = vOut.ShadowMapSamplingPos;
	gl_TexCoord[1] = vOut.MVar;
}

#else

#ifdef DIRECT3D_11
float4x4 mWorldTrans;
#endif
float4x4 mWorldViewProj;
float4x4 mWorldViewProj2;
float3 LightPos;
float MaxD;
float MAPRES;

struct VS_OUTPUT
{
	float4 Position				: SV_Position;
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
	
	#ifdef DIRECT3D_11
	float4 worldpos = mul(float4(In.Position.x, In.Position.y, In.Position.z, 1.0), mWorldTrans);
	float3 LightDir = normalize(LightPos - worldpos.xyz);
	#else
	float3 LightDir = normalize(LightPos - In.Position);
	#endif
		
	OUT.MVar = float4(SMPos.z, dot(In.Normal, LightDir), 1.0 / MAPRES, MaxD);
	
	return (OUT);
}

#endif