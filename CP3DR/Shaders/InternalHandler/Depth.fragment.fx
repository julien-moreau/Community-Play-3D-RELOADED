
#ifdef OPENGL_DRIVER

#ifdef ALPHA_ENABLED
uniform sampler2D ColorMapSampler;
#endif

void main() 
{
	vec4 vInfo = gl_TexCoord[0];
	float depth = vInfo.z / vInfo.x;
	float alpha = 1.0;

#ifdef ALPHA_ENABLED
	alpha = texture2D(ColorMapSampler, gl_TexCoord[1].xy).a;
#endif

	gl_FragColor = vec4(depth, depth * depth, 0.0, alpha);
}

#else

#include "Shaders/InternalHandler/Utils.hlsl.fx"

#ifdef ALPHA_ENABLED
CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);
#endif

struct VS_OUTPUT
{
	float4 Position: POSITION0;
	float4 ClipPos: TEXCOORD0;
	float2 Texcoords: TEXCOORD1;
	float4 VColor: TEXCOORD2;
};

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float depth = In.ClipPos.z / In.ClipPos.x;
	float alpha = 0.0;

	#ifdef ALPHA_ENABLED
	alpha = CP3DTex2D(ColorMapSampler, In.Texcoords.xy, ColorMapSamplerST).a;
	#endif

	return float4(depth, depth * depth, 0.0, alpha);
}

#endif
