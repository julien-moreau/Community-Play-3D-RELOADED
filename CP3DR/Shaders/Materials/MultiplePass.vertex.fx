
##ifdef OPENGL_DRIVER



##else

float4x4 WorldViewProj;
float4x4 WorldView;

struct VertexShaderInput {
	float4 Position : POSITION0;
	float4 TexCoord : TEXCOORD0;

	float3 Normal : NORMAL;
	float3 Tangent : TANGENT0;
};

struct VertexShaderOutput {
	float4 Position : POSITION0;
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

##endif
