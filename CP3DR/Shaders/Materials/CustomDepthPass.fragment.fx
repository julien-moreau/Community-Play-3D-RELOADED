
#ifdef OPENGL_DRIVER

uniform float MaxD[__CP3D__PASSES_COUNT__];

void main(void) {
	__CP3D__PIXEL_MAIN__
}

#else

float MaxD[__CP3D__PASSES_COUNT__];

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float4 ClipPos: TEXCOORD0;
	float2 Texcoords: TEXCOORD1;
	float4 VColor: TEXCOORD2;
};

struct PS_OUTPUT {
	__CP3D__RENDER_TARGETS__
};

PS_OUTPUT pixelMain(VS_OUTPUT In) {
	PS_OUTPUT OUT = (PS_OUTPUT)0;

	__CP3D__PIXEL_MAIN__

	return (OUT);
}

#endif
