#ifdef OPENGL_DRIVER
#error "These utils functions are not intended for OpenGL Drivers"
#endif

#ifdef DIRECT3D_11
typedef Texture2D CP3DTexture;
#else
typedef sampler2D CP3DTexture;
#endif

//-----------------------------------------------------------------
// Texture2D & sampler2D
//-----------------------------------------------------------------

/*
Common usage :

#ifdef DIRECT3D_11
Texture2D tex : register(t0);
#else
sampler2D tex : register(s0);
#endif

SamplerState st : register(s0); // Ignored if D3D9 driver.

float4 pixelMain(...) : COLOR0
{
	// If D3D9 driver, st will be ignored, but will work with D3D11 driver :)
	// It ensures a full compatibility between D3D9 and D3D11 drivers
	return CP3DTex2D(tex, coord.xy, st);
}
*/

// Related to D3D9 driver
inline float4 CP3DTex2D(sampler2D s, float2 t, SamplerState st) {
	return tex2D(s, t);
}

// Related to D3D11 driver
inline float4 CP3DTex2D(Texture2D s, float2 t, SamplerState st) {
	return s.Sample(st, t);
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
