
#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler; 

void main() 
{		
	vec4 finalCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
	gl_FragColor = finalCol;
}

#else

#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float2 TexCoords	: TEXCOORD0;
	float3 LStart		: TEXCOORD1;
	float3 LEnd			: TEXCOORD2;
	float2 ScreenSize	: TEXCOORD3;
};

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float4 finalCol = CP3DTex2D(ColorMapSampler, In.TexCoords, ColorMapSamplerST);

	return finalCol;
}

#endif
