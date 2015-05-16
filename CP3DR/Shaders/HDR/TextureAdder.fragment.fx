#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;
uniform sampler2D UserMapSampler;

void main()
{
	vec4 sum = texture2D(ColorMapSampler, gl_TexCoord[0].xy) + texture2D(UserMapSampler, gl_TexCoord[0].xy);
	sum.a = clamp(sum.a, 0.0, 1.0);

	gl_FragColor = sum;
}

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

CP3DTexture UserMapSampler : registerTexture(t3);
SamplerState UserMapSamplerST : register(s3);

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	float4 sum = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST) + CP3DTex2D(UserMapSampler, In.TexCoords.xy, UserMapSamplerST);
	sum.a = saturate(sum.a);

	return sum;
}

#endif
