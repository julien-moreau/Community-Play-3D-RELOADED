#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;
uniform vec2 dsOffsets[16];

void main()
{
	vec4 average = vec4(0.0, 0.0, 0.0, 0.0);

	for (int i = 0; i < 16; i++) {
		average += texture2D(ColorMapSampler, gl_TexCoord[0].xy + dsOffsets[i]);
	}

	average /= 16.0;

	gl_FragColor = average;
}

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

float2 dsOffsets[16];

float4 pixelMain(VS_OUTPUT In) : COLOR0{
	float4 average = float4(0.0, 0.0, 0.0, 0.0);

	for (int i = 0; i < 16; i++) {
		average += CP3DTex2D(ColorMapSampler, In.TexCoords.xy + dsOffsets[i], ColorMapSamplerST);
	}

	average /= 16.0;

	return average;
}

#endif
