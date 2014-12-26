
#ifdef OPENGL_DRIVER

uniform sampler2D diffuseSampler;
#ifdef _CP3D_SOLID_2_LAYER_
uniform sampler2D blendSampler;
#endif

void main(void) {
	vec4 diffuseColor = texture2D(diffuseSampler, gl_TexCoord[0].xy);

	#ifdef _CP3D_SOLID_2_LAYER_
	gl_FragColor = diffuseColor * texture2D(blendSampler, gl_TexCoord[0].xy);
	#else
	gl_FragColor = diffuseColor;
	#endif
}

#else

#ifdef DIRECT3D_11
	Texture2D diffuseSampler : register(t0);
	SamplerState diffuseSamplerST : register(s0);
	#ifdef _CP3D_SOLID_2_LAYER_
	Texture2D blendSampler : register(t1);
	SamplerState blendSamplerST : register(s1);
	#endif
#else
	sampler2D diffuseSampler : register(s0);
	#ifdef _CP3D_SOLID_2_LAYER_
	sampler2D blendSampler : register(s1);
	#endif
#endif

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float4 TexCoord : TEXCOORD0;
};

float4 pixelMain(VS_OUTPUT input) : COLOR0 {
	#ifdef DIRECT3D_11
	float4 diffuseColor = diffuseSampler.Sample(diffuseSamplerST, input.TexCoord.xy);
	#else
	float4 diffuseColor = tex2D(diffuseSampler, input.TexCoord.xy);
	#endif

	#ifdef _CP3D_SOLID_2_LAYER_
		#ifdef DIRECT3D_11
		return diffuseColor * blendSampler.Sample(blendSamplerST, input.TexCoord.xy);
		#else
		return diffuseColor * tex2D(blendSampler, input.TexCoord.xy);
		#endif
	#else
	return diffuseColor;
	#endif
}

#endif
