#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;

uniform vec2 dsOffsets[9];
uniform float halfDestPixelSize;

#ifdef FINAL_DOWN_SAMPLER
vec4 pack(float value) {
	const vec4 bit_shift = vec4(255.0 * 255.0 * 255.0, 255.0 * 255.0, 255.0, 1.0);
	const vec4 bit_mask = vec4(0.0, 1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0);

	vec4 res = fract(value * bit_shift);
	res -= res.xxyz * bit_mask;

	return res;
}
#endif

void main()
{
	vec4 color = vec4(0.0);
	float average = 0.0;

	for (int i = 0; i < 9; i++)
	{
		color = texture2D(ColorMapSampler, gl_TexCoord[0].xy + vec2(halfDestPixelSize, halfDestPixelSize) + dsOffsets[i]);
		average += color.r;
	}

	average /= 9.0;

	#ifdef FINAL_DOWN_SAMPLER
	gl_FragColor = pack(average);
	#else
	gl_FragColor = vec4(average);
	#endif
}


#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

float2 dsOffsets[9];
float halfDestPixelSize;

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	float4 color = 0.0;
	float average = 0.0;

	for (int i = 0; i < 9; i++)
	{
		color = CP3DTex2D(ColorMapSampler, In.TexCoords.xy + float2(halfDestPixelSize, halfDestPixelSize) + dsOffsets[i], ColorMapSamplerST);
		average += color.r;
	}

	average /= 9.0;

	return average;
}

#endif
