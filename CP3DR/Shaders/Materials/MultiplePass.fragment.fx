
##ifdef OPENGL_DRIVER



##else

/// Light Scattering pass
bool LSRenderNormal;
sampler2D BaseSampler : register(s0);

/// Normal pass
float FarDistance;
sampler2D NormalSampler : register(s1);

struct VertexShaderInput {
	float4 TexCoord : TEXCOORD0;

	float3 ViewPos : TEXCOORD1;
	float3 Normal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 BiNormal : TEXCOORD4;
};

struct PixelOutput {
	float4 NormalTarget	 : COLOR0;
	float4 LSTarget		 : COLOR1;
};

PixelOutput pixelMain(VertexShaderInput input) {
	PixelOutput output = (PixelOutput)0;

	/// Light Scattering pass
	if (LSRenderNormal)
		output.LSTarget = tex2D(BaseSampler, input.TexCoord.xy);
	else
		output.LSTarget = float4(0.0, 0.0, 0.0, 0.0);

	/// Normal pass
	float4 texNormal = (tex2D(NormalSampler, input.TexCoord) - 0.5) * 2.0;
	float3x3 normalRotation = float3x3(input.Tangent, input.BiNormal, input.Normal);
	output.NormalTarget.rgb = normalize( mul(texNormal.xyz, normalRotation) );
	output.NormalTarget.a = length(input.ViewPos) / FarDistance;

	return (output);
};

##endif
