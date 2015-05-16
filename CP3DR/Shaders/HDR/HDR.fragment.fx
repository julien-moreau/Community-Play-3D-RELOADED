#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;

uniform float exposure;
uniform float averageLuminance;

void main()
{
	vec4 color = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
	vec4 adjustedColor = color / averageLuminance * exposure;

	color = adjustedColor;
	color.a = 1.0;
	gl_FragColor = color;
}

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

float exposure;
float averageLuminance;

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	float4 color = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST);
	float4 adjustedColor = color / averageLuminance * exposure;

	color = adjustedColor;
	color.a = 1.0;
	return color;
}

#endif
