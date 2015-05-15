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

float blurOffsets[9];
float blurWeights[9];

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	float4 color = float4(0.0, 0.0, 0.0, 0.0);

	for (int i = 0; i < 9; i++) {
		#ifdef GAUSSIAN_BLUR_H
		color += CP3DTex2D(ColorMapSampler, In.TexCoords.xy + float2(blurOffsets[i], 0.0), ColorMapSamplerST) * blurWeights[i];
		#else
		color += CP3DTex2D(ColorMapSampler, In.TexCoords.xy + float2(0.0, blurOffsets[i]), ColorMapSamplerST) * blurWeights[i];
		#endif
	}

	color.a = 1.0;
	return color;
}

#endif
