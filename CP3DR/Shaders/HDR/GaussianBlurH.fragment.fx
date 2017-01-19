#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;
uniform float blurOffsets[9];
uniform float blurWeights[9];
uniform float blurWidth;

void main()
{
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

	for (int i = 0; i < 9; i++) {
		#ifdef GAUSSIAN_BLUR_H
		color += texture2D(ColorMapSampler, clamp(gl_TexCoord[0].xy + vec2(blurOffsets[i] * blurWidth, 0.0), vec2(0.0, 0.0), vec2(1.0, 1.0))) * blurWeights[i];
		color += texture2D(ColorMapSampler, clamp(gl_TexCoord[0].xy - vec2(blurOffsets[i] * blurWidth, 0.0), vec2(0.0, 0.0), vec2(1.0, 1.0))) * blurWeights[i];
		#else
		color += texture2D(ColorMapSampler, clamp(gl_TexCoord[0].xy + vec2(0.0, blurOffsets[i] * blurWidth), vec2(0.0, 0.0), vec2(1.0, 1.0))) * blurWeights[i];
		color += texture2D(ColorMapSampler, clamp(gl_TexCoord[0].xy - vec2(0.0, blurOffsets[i] * blurWidth), vec2(0.0, 0.0), vec2(1.0, 1.0))) * blurWeights[i];
		#endif
	}

	color.a = 1.0;
	gl_FragColor = color;
}

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

float blurOffsets[9];
float blurWeights[9];
float blurWidth;

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	float4 color = float4(0.0, 0.0, 0.0, 0.0);

	[unroll]
	for (int i = 0; i < 9; i++) {
		#ifdef GAUSSIAN_BLUR_H
		color += CP3DTex2D(ColorMapSampler, clamp(In.TexCoords.xy + float2(blurOffsets[i] * blurWidth, 0.0), float2(0.0, 0.0), float2(1.0, 1.0)), ColorMapSamplerST) * blurWeights[i];
		color += CP3DTex2D(ColorMapSampler, clamp(In.TexCoords.xy - float2(blurOffsets[i] * blurWidth, 0.0), float2(0.0, 0.0), float2(1.0, 1.0)), ColorMapSamplerST) * blurWeights[i];
		#else
		color += CP3DTex2D(ColorMapSampler, clamp(In.TexCoords.xy + float2(0.0, blurOffsets[i] * blurWidth), float2(0.0, 0.0), float2(1.0, 1.0)), ColorMapSamplerST) * blurWeights[i];
		color += CP3DTex2D(ColorMapSampler, clamp(In.TexCoords.xy - float2(0.0, blurOffsets[i] * blurWidth), float2(0.0, 0.0), float2(1.0, 1.0)), ColorMapSamplerST) * blurWeights[i];
		#endif
	}

	color.a = 1.0;
	return color;
}

#endif
