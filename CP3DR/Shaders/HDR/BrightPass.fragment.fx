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

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float4 finalVal = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST);
	finalVal *= 2.0;
	finalVal = pow(finalVal, 8.0);

	return finalVal;
}

#endif
