
#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler; 

vec2 offsetArray[5];

void main() 
{

	#ifdef VERTICAL_VSM_BLUR
	offsetArray[0] = vec2(0.0, 0.0);
	offsetArray[1] = vec2(0.0, -1.5 / gl_TexCoord[3].y);
	offsetArray[2] = vec2(0.0, 1.5 / gl_TexCoord[3].y);
	offsetArray[3] = vec2(0.0, -2.5 / gl_TexCoord[3].y);
	offsetArray[4] = vec2(0.0, 2.5 / gl_TexCoord[3].y);
	#else
	offsetArray[0] = vec2(0.0, 0.0);
	offsetArray[1] = vec2(-1.5 / gl_TexCoord[3].x, 0.0);
	offsetArray[2] = vec2(1.5 / gl_TexCoord[3].x, 0.0);
	offsetArray[3] = vec2(-2.5 / gl_TexCoord[3].x, 0.0);
	offsetArray[4] = vec2(2.5 / gl_TexCoord[3].x, 0.0);
	#endif

	vec4 BlurCol = vec4(0.0, 0.0, 0.0, 0.0);

	for(int i = 0;i < 5;++i)
		BlurCol += texture2D(ColorMapSampler, clamp(gl_TexCoord[0].xy + offsetArray[i], vec2(0.001, 0.001), vec2(0.999, 0.999)));

	gl_FragColor = BlurCol / 5.0;
}

#else

#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ColorMapSampler : registerTexture(t0);
SamplerState ColorMapSamplerST : register(s0);

struct VS_OUTPUT
{
	float4 Position		: SV_Position;
	float2 TexCoords	: TEXCOORD0;
	float3 LStart		: TEXCOORD1;
	float3 LEnd			: TEXCOORD2;
	float2 ScreenSize	: TEXCOORD3;
};

float4 pixelMain (VS_OUTPUT In) : COLOR0
{
	float2 offsetArray[5];
	#ifdef VERTICAL_VSM_BLUR
	offsetArray[0] = float2(0, 0);
	offsetArray[1] = float2(0, 1.5 / In.ScreenSize.y);
	offsetArray[2] = float2(0, -1.5 / In.ScreenSize.y);
	offsetArray[3] = float2(0, 2.5 / In.ScreenSize.y);
	offsetArray[4] = float2(0, -2.5 / In.ScreenSize.y);
	#else
	offsetArray[0] = float2(0, 0);
	offsetArray[1] = float2(1.5 / In.ScreenSize.x, 0);
	offsetArray[2] = float2(-1.5 / In.ScreenSize.x, 0);
	offsetArray[3] = float2(2.5 / In.ScreenSize.x, 0);
	offsetArray[4] = float2(-2.5 / In.ScreenSize.x, 0);
	#endif

	float4 finalVal = float4(0.0, 0.0, 0.0, 0.0);

		for (int i = 0; i < 5; ++i)
			finalVal += CP3DTex2D(ColorMapSampler, clamp(In.TexCoords.xy + offsetArray[i], float2(0.001, 0.001), float2(0.999, 0.999)), ColorMapSamplerST);

	return finalVal / 5.0;
}

#endif
