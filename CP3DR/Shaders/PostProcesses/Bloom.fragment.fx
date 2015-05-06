#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;

void main() 
{		
	gl_FragColor = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
}

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

CP3DTexture ScreenMapSampler : registerTexture(t1);
SamplerState ScreenMapSamplerST : register(s1);

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float4 screenCol = CP3DTex2D(ScreenMapSampler, In.TexCoords.xy, ScreenMapSamplerST);
	float4 bloomCol = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST);

	return(screenCol * 0.9 + bloomCol * 0.5);
}

#endif
