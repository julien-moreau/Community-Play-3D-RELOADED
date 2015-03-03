
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

struct VS_OUTPUT
{
	float4 Position		: SV_Position;

	#if defined(CP3D_COMPUTE_DEPTH_MATERIAL)
	float4 ClipPos 		: TEXCOORD0;
	#else
	float2 TexCoords 	: TEXCOORD0;
	#endif

	#if !defined(CP3D_COMPUTE_DEPTH_MATERIAL)
	float2 TexCoords2 	: TEXCOORD1;
	#endif
};

float4 pixelMain(VS_OUTPUT input) : COLOR0
{
	return computeDepthPixel(input.ClipPos);
}

#endif
