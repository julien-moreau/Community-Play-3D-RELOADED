
#ifdef OPENGL_DRIVER

uniform mat4 WorldView;

void main()
{
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_TexCoord[1] = WorldView * vec4(gl_Normal.xyz, 0.0); // Normal
	gl_TexCoord[2] = WorldView * vec4(gl_MultiTexCoord1.xyz, 0.0); // Tangent
	gl_TexCoord[3].xyz = cross(gl_TexCoord[1].xyz, gl_TexCoord[2].xyz); // BiNormal
	gl_TexCoord[4] = WorldView * gl_Position; /// View Pos
}

#else

cbuffer cbParams : register(b0)
{
	float4x4 WorldViewProj;
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
};

VertexShaderOutput vertexMain(VertexShaderInput input) {
	VertexShaderOutput output = (VertexShaderOutput)0;

	/// General
	output.Position = mul(input.Position, WorldViewProj);
	output.TexCoord = input.TexCoord;

	/// Normal
	output.Normal = mul(WorldView, float4(input.Normal, 0.0)).xyz;
	output.Tangent = mul(WorldView, float4(input.Tangent, 0.0)).xyz;
	output.BiNormal = cross(output.Normal, output.Tangent);
	output.ViewPos = mul(WorldView, input.Position).xyz;

	return (output);
}

#endif
