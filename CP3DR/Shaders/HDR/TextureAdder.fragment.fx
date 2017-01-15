#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;
uniform sampler2D UserMapSampler;

void main()
{
	/*vec4 sum = texture2D(ColorMapSampler, gl_TexCoord[0].xy) + texture2D(UserMapSampler, gl_TexCoord[0].xy);
	sum.a = clamp(sum.a, 0.0, 1.0);

	gl_FragColor = sum;
	*/
	vec4 blurColor = texture2D(UserMapSampler, gl_TexCoord[0].xy);
	blurColor += texture2D(ScreenMapSampler, gl_TexCoord[0].xy) * length(blurColor.rgb);

	vec3 X = max(vec3(0.0, 0.0, 0.0), blurColor.rgb - 0.004);
	vec3 retColor = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);

	gl_FragColor = vec4(texture2D(ColorMapSampler, gl_TexCoord[0].xy).rgb + retColor * retColor, 1.0);
}

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

CP3DTexture LensSampler : registerTexture(t1);
SamplerState LensSamplerST : register(s0);

CP3DTexture UserMapSampler : registerTexture(t3);
SamplerState UserMapSamplerST : register(s3);

float4 pixelMain(VS_OUTPUT In) : COLOR0 {
	/*
	float4 sum = CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST) + CP3DTex2D(UserMapSampler, In.TexCoords.xy, UserMapSamplerST);
	sum.a = saturate(sum.a);

	return sum;
	*/
	float4 blurColor = CP3DTex2D(UserMapSampler, In.TexCoords.xy, UserMapSamplerST);
	blurColor += CP3DTex2D(LensSampler, In.TexCoords.xy, LensSamplerST) * length(blurColor.rgb);

	float3 X = max(float3(0.0, 0.0, 0.0), blurColor.rgb - 0.004);
	float3 retColor = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);

	return float4(CP3DTex2D(ColorMapSampler, In.TexCoords.xy, ColorMapSamplerST).rgb + retColor * retColor, 1.0);
}

#endif
