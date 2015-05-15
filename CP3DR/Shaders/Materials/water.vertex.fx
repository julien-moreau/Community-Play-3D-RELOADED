
#ifdef OPENGL_DRIVER



#else

cbuffer cbParams : register(c0)
{
	float4x4 worldViewProj;
	float4x4 worldReflectionViewProj;
	float waveLength;
	float time;
	float windForce;
	float2 windDirection;
};

struct VS_OUTPUT
{
	float4 Position				 : SV_Position;
	float2 BumpMapTexCoord		 : TEXCOORD0;
	float3 RefractionMapTexCoord : TEXCOORD1;
	float3 ReflectionMapTexCoord : TEXCOORD2;
	float3 Position3D			 : TEXCOORD3;
};

struct VS_INPUT
{
	float4 Position	: POSITION;
	float4 Color	: COLOR0;
	float2 TexCoord	: TEXCOORD0;
};

VS_OUTPUT vertexMain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 pos = mul(input.Position, worldViewProj);
	output.Position = pos;

	output.BumpMapTexCoord = input.TexCoord / waveLength + time * windForce * windDirection;

	output.RefractionMapTexCoord.x = 0.5 * (pos.w + pos.x);
	output.RefractionMapTexCoord.y = 0.5 * (pos.w - pos.y);
	output.RefractionMapTexCoord.z = pos.w;

	pos = mul(input.Position, worldReflectionViewProj);
	output.ReflectionMapTexCoord.x = 0.5 * (pos.w + pos.x);
	output.ReflectionMapTexCoord.y = 0.5 * (pos.w - pos.y);
	output.ReflectionMapTexCoord.z = pos.w;

	output.Position3D = input.Position;

	return output;
}

#endif
