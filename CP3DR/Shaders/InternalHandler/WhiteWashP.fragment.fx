
#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;

void main() 
{
	float alpha = texture2D(ColorMapSampler, gl_TexCoord[1].xy).a;

    gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);
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

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float alpha = CP3DTex2D(ColorMapSampler, In.Texcoords, ColorMapSamplerST).a;
	return float4(1.0, 1.0, 1.0, alpha);
}

#endif
