
#ifdef OPENGL_DRIVER

uniform mat4 WorldView;
uniform mat4 PrevWorldViewProj;

void main()
{
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;

	// Normal
	gl_TexCoord[1] = WorldView * vec4(gl_Normal.xyz, 0.0); // Normal
	gl_TexCoord[2] = WorldView * vec4(gl_MultiTexCoord1.xyz, 0.0); // Tangent
	gl_TexCoord[3].xyz = cross(gl_TexCoord[1].xyz, gl_TexCoord[2].xyz); // BiNormal
	gl_TexCoord[4] = WorldView * gl_Position; /// View Pos

	// Velocity
	gl_TexCoord[5] = gl_Position;
	gl_TexCoord[6] = PrevWorldViewProj * gl_Vertex;
}

#else

cbuffer cbParams : register(b0)
{
	float4x4 WorldViewProj;
	float4x4 PrevWorldViewProj;
	float4x4 WorldView;
};

struct VertexShaderInput {
	float4 Position : POSITION0;
	float4 TexCoord : TEXCOORD0;

	float3 Normal : NORMAL;
	float3 Tangent : TANGENT0;
};

struct VertexShaderOutput {
	float4 Position : SV_Position;
	float4 TexCoord : TEXCOORD0;

	float3 ViewPos : TEXCOORD1;
	float3 Normal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 BiNormal : TEXCOORD4;
	float4 CurPosition : TEXCOORD5;
	float4 PrevPosition : TEXCOORD6;
};

VertexShaderOutput vertexMain(VertexShaderInput input) {
	VertexShaderOutput output = (VertexShaderOutput)0;

	/// General
	output.Position = mul(input.Position, WorldViewProj);
	output.TexCoord = input.TexCoord;

	/// Normal
	output.Normal = mul(float4(input.Normal, 0.0), WorldView).xyz;
	output.Tangent = mul(float4(input.Tangent, 0.0), WorldView).xyz;
	output.BiNormal = cross(output.Normal, output.Tangent);
	output.ViewPos = mul(input.Position, WorldView).xyz;

	/// Velocity
	output.CurPosition = mul(input.Position, WorldViewProj);
	output.PrevPosition = mul(input.Position, PrevWorldViewProj);

	return output;
}

#endif
