#define CP3D_MATERIAL 
#define DIRECT3D_9 

#ifdef OPENGL_DRIVER

uniform int LSRenderNormal;
uniform sampler2D BaseSampler;

uniform float FarDistance;
uniform sampler2D NormalSampler;

void main() {
	/// Light Scattering
	if (LSRenderNormal == 1)
		gl_FragData[1] = texture2D(BaseSampler, gl_TexCoord[0].xy);
	else
		gl_FragData[1] = vec4(0.0, 0.0, 0.0, 0.0);

	/// Normal pass
	vec4 texNormal = (texture2D(NormalSampler, gl_TexCoord[0].xy) - 0.5) * 2.0;
	mat3 normalRotation = mat3(gl_TexCoord[2].xy, gl_TexCoord[2].z, gl_TexCoord[3].xy, gl_TexCoord[3].z, gl_TexCoord[1].xy, gl_TexCoord[1].z);

	gl_FragData[0].rgb = normalize(texNormal.xyz * normalRotation);
	gl_FragData[0].a = length(gl_TexCoord[4]) / FarDistance;
}

#else

/// Light Scattering pass
bool LSRenderNormal;
#ifdef DIRECT3D_11
Texture2D BaseSampler : register(t0);
SamplerState BaseSamplerST : register(s0);
#else
sampler2D BaseSampler : register(s0);
#endif

/// Normal pass
float FarDistance;
#ifdef DIRECT3D_11
Texture2D NormalSampler : register(t1);
SamplerState NormalSamplerST : register(s1);
#else
sampler2D NormalSampler : register(s1);
#endif

/// Velocity
//float velocityFactor;

struct VertexShaderOutput
{
	float4 Position : SV_Position;
	float4 TexCoord : TEXCOORD0;

	float3 ViewPos : TEXCOORD1;
	float3 Normal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 BiNormal : TEXCOORD4;
	float4 CurPosition : TEXCOORD5;
	float4 PrevPosition : TEXCOORD6;
};

struct PixelOutput
{
	float4 NormalTarget	 : COLOR0;
	float4 LSTarget		 : COLOR1;
	float4 Velocity      : COLOR2;
};

PixelOutput pixelMain(VertexShaderOutput input) {
	PixelOutput output = (PixelOutput)0;

	/// Light Scattering pass
	if (LSRenderNormal) {
		#ifdef DIRECT3D_11
		output.LSTarget = BaseSampler.Sample(BaseSamplerST, input.TexCoord.xy);
		#else
		output.LSTarget = tex2D(BaseSampler, input.TexCoord.xy);
		#endif
	}
	else
		output.LSTarget = float4(0.0, 0.0, 0.0, 0.0);

	/// Normal pass
	#ifdef DIRECT3D_11
	float4 texNormal = (NormalSampler.Sample(NormalSamplerST, input.TexCoord.xy) - 0.5) * 2.0;
	#else
	float4 texNormal = (tex2D(NormalSampler, input.TexCoord) - 0.5) * 2.0;
	#endif
	float3x3 normalRotation = float3x3(input.Tangent, input.BiNormal, input.Normal);
	output.NormalTarget.rgb = normalize( mul(texNormal.xyz, normalRotation) );
	output.NormalTarget.a = 1.0; // length(input.ViewPos) / FarDistance;

	/// Velocity
	float2 a = (input.CurPosition.xy / input.CurPosition.w) * 0.5 + 0.5;
	float2 b = (input.PrevPosition.xy / input.PrevPosition.w) * 0.5 + 0.5;
	float2 velocity = (a - b) * 0.5 + 0.5;
	velocity *= 0.5 + 0.5;
	velocity = pow(velocity, 3.0);

	output.Velocity = float4(velocity, 0.0, 1.0);

	return (output);
};

#endif
