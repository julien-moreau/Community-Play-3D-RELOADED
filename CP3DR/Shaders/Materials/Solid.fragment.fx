
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

#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture diffuseSampler : register(t0);
SamplerState diffuseSamplerST : register(s0);

#ifdef _CP3D_SOLID_2_LAYER_
CP3DTexture blendSampler : register(t1);
SamplerState blendSamplerST : register(s1);
#endif

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float4 TexCoord : TEXCOORD0;
};

float4 pixelMain(VS_OUTPUT input) : COLOR0
{
	float4 diffuseColor = CP3DTex2D(diffuseSampler, input.TexCoord.xy, diffuseSamplerST);

	#ifdef _CP3D_SOLID_2_LAYER_
	return diffuseColor * CP3DTex2D(blendSampler, input.TexCoord.xy, blendSamplerST);
	#else
	return diffuseColor;
	#endif
}

#endif
