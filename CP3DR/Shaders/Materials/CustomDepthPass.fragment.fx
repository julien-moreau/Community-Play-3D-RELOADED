
#ifdef OPENGL_DRIVER

uniform float MaxD[__CP3D__PASSES_COUNT__];

void main(void) {
	__CP3D__PIXEL_MAIN__
}

#else

float MaxD[__CP3D__PASSES_COUNT__];

struct PS_OUTPUT {
	__CP3D__RENDER_TARGETS__
};

PS_OUTPUT pixelMain(float4 ClipPos: TEXCOORD0) {
	PS_OUTPUT OUT = (PS_OUTPUT)0;

	__CP3D__PIXEL_MAIN__

	return (OUT);
}

#endif
