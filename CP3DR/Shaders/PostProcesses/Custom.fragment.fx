#ifdef OPENGL_DRIVER

uniform sampler2D UserMapSampler;

void main() 
{		
	vec4 col = texture2D(UserMapSampler, gl_TexCoord[0].xy);

	gl_FragColor = col;
}

#else

#define POST_PROCESS
#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture UserMapSampler : registerTexture(t3);
SamplerState UserMapSamplerST : register(s3);

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float4 col = CP3DTex2D(UserMapSampler, In.TexCoords.xy, UserMapSamplerST);

	return col;
}

#endif
