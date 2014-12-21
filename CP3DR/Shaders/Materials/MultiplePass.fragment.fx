
##ifdef OPENGL_DRIVER



##else

/// Light Scattering pass
bool LSRenderNormal;
##ifdef DIRECT3D_11
Texture2D BaseSampler : register(t0);
SamplerState BaseSamplerST : register(s0);
##else
sampler2D BaseSampler : register(s0);
##endif

/// Normal pass
float FarDistance;
##ifdef DIRECT3D_11
Texture2D NormalSampler : register(t1);
SamplerState NormalSamplerST : register(s1);
##else
sampler2D NormalSampler : register(s1);
##endif

struct VertexShaderOutput {
	float4 Position : SV_Position;
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

PixelOutput pixelMain(VertexShaderOutput input) {
	PixelOutput output = (PixelOutput)0;

	/// Light Scattering pass
	if (LSRenderNormal) {
		##ifdef DIRECT3D_11
		output.LSTarget = BaseSampler.Sample(BaseSamplerST, input.TexCoord.xy);
		##else
		output.LSTarget = tex2D(BaseSampler, input.TexCoord.xy);
		##endif
	}
	else
		output.LSTarget = float4(0.0, 0.0, 0.0, 0.0);

	/// Normal pass
	##ifdef DIRECT3D_11
	float4 texNormal = (NormalSampler.Sample(NormalSamplerST, input.TexCoord.xy) - 0.5) * 2.0;
	##else
	float4 texNormal = (tex2D(NormalSampler, input.TexCoord) - 0.5) * 2.0;
	##endif
	float3x3 normalRotation = float3x3(input.Tangent, input.BiNormal, input.Normal);
	output.NormalTarget.rgb = normalize( mul(texNormal.xyz, normalRotation) );
	output.NormalTarget.a = length(input.ViewPos) / FarDistance;

	return (output);
};

##endif
