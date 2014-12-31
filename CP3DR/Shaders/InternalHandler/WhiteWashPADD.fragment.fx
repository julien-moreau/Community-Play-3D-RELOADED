
#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;

float luminance(vec3 color)
{
	return clamp(color.r * 0.3 + color.g * 0.59 + color.b * 0.11, 0.0, 1.0);
}

void main() 
{
	vec4 diffuseTex = texture2D(ColorMapSampler, gl_TexCoord[1].xy);
    gl_FragColor = vec4(1.0, 1.0, 1.0, luminance(diffuseTex.rgb));
}

#else

#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : register(t0);
SamplerState ColorMapSamplerST : register(s0);

struct VS_OUTPUT
{
	float4 Position: POSITION;
	float4 ClipPos: TEXCOORD0;
	float2 Texcoords: TEXCOORD1;
	float4 VColor: TEXCOORD2;
};

float luminance(float3 color)
{
	return clamp(color.r * 0.3 + color.g * 0.59 + color.b * 0.11, 0.0, 1.0);
}

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float4 diffuseTex = CP3DTex2D(ColorMapSampler, In.Texcoords, ColorMapSamplerST);
	diffuseTex *= In.VColor;

	return float4(1.0, 1.0, 1.0, luminance(diffuseTex.rgb));
}

#endif
