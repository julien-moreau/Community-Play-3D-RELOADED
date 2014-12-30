#ifdef OPENGL_DRIVER
#error "These utils functions are not intended for OpenGL Drivers"
#endif

#undef OPENGL_DRIVER

/*
Redefines the Texture2D and sampler2D to ensure a
compatibility between D3D9 & D3D11

Usage :

CP3DTexture myTexture : registerTexture(t0);
// - if D3D9, then t0 is replaced by s0
// - if D3D11, then nothing replaced

*/
#ifdef DIRECT3D_11
typedef Texture2D CP3DTexture;
#else
typedef sampler2D CP3DTexture;
#define t0 s0
#define t1 s1
#define t2 s2
#define t3 s3
#define t4 s4
#define t5 s5
#define t6 s6
#define t7 s7
#endif
#define registerTexture(reg) register(reg)

//-----------------------------------------------------------------
// Texture2D & sampler2D
//-----------------------------------------------------------------

/*
Common usage :


CP3DTexture tex : registerTexture(t0);
SamplerState st : register(s0); // Ignored if D3D9 driver.

float4 pixelMain(...) : COLOR0
{
	// If D3D9 driver, st will be ignored, but will work with D3D11 driver :)
	// It ensures a full compatibility between D3D9 and D3D11 drivers
	return CP3DTex2D(tex, coords.xy, st);
}
*/

// Related to D3D9 driver
inline float4 CP3DTex2D(CP3DTexture s, float2 t, SamplerState st) {
	#ifdef DIRECT3D_11
	return s.Sample(st, t);
	#else
	return tex2D(s, t);
	#endif
}

//-----------------------------------------------------------------
// Post-Processes
//-----------------------------------------------------------------

/*
Defines the output values of the screen quad's vertex program.
Common usage in your post-process pixel shader program
(access the output values using the following example) :

...

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	return CP3DTex2D(myTexture, In.TexCoords.xy, st);
}

*/
#ifdef POST_PROCESS
struct VS_OUTPUT
{
	float4 Position		: SV_Position;
	float2 TexCoords	: TEXCOORD0;
	float3 LStart		: TEXCOORD1;
	float3 LEnd			: TEXCOORD2;
	// ScreenSize also accessible by defines automatically setted :
	/*
	#define SCREENX ...
	#define SCREENY ...

	Then you can put in your code anywhere :
		float2 screensize = float2(SCREENX, SCREENY);
	Or
		float pixels = SCREENX * SCREENY
	Etc.
	*/
	float2 ScreenSize	: TEXCOORD3;
};
#endif
