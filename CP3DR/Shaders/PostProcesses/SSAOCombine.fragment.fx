#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

void main() 
{		
	vec4 screenCol = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);
	vec4 SSAOCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);

	gl_FragColor = (screenCol * SSAOCol);
}

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
CP3DTexture ScreenMapSampler : registerTexture(t1);

SamplerState ColorMapSamplerST : register(s0);
SamplerState ScreenMapSamplerST : register(s1);

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float4 screenCol = CP3DTex2D(ScreenMapSampler, In.TexCoords.xy, ScreenMapSamplerST);
	float4 SSAOCol = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST);

	return(screenCol * SSAOCol);
}

#endif
