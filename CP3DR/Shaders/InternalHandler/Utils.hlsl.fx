#ifndef __FX_UTILS_HLSL_FX_INCLUDED_
#define __FX_UTILS_HLSL_FX_INCLUDED_

#ifdef OPENGL_DRIVER
#error "These utils functions are not intended for OpenGL Drivers"
#endif

#undef OPENGL_DRIVER

//-----------------------------------------------------------------
// Compatibilyty D3D9 & D3D11
//-----------------------------------------------------------------
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

[...]

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

/*
If CP3D_MATERIAL and CP3D_COMPUTE_DEPTH_MATERIAL are defined,
you're able to compute your own depth material passing the final 3D positions
to the vertex program, and depth parameters to the pixel program.
Simply use the cp3d::rendering::ICP3DMaterialCreator and add the
CP3D_COMPUTE_DEPTH_MATERIAL define.

See depthExample.xxx.fx in "Shaders/Materials/" for examples.
*/
#if defined(CP3D_MATERIAL) && defined(CP3D_COMPUTE_DEPTH_MATERIAL)
/*
Let you compute the depth vertex function
*/
inline float4 computeDepthVertex(float4 p, float maxDistance) {
	float4 clipPos = p;
	clipPos.x = maxDistance;

	return clipPos;
}

/*
Let you compute the depth pixel function
*/
#ifdef DEPTH_PIXEL_ALPHA_ENABLED
inline float4 computeDepthPixel(float4 p, CP3DTexture t, float2 tc, SamplerState st) {
#else
inline float4 computeDepthPixel(float4 p) {
#endif
	float depth = p.z / p.x;
	float alpha = 0.0;

	#ifdef DEPTH_PIXEL_ALPHA_ENABLED
	alpha = CP3DTex2D(t, tc.xy, st).a;
	#endif

	return float4(depth, depth * depth, 0.0, 0.0);
}
#endif

/*
If CP3D_MATERIAL and CP3D_COMPUTE_SHADOWS_MATERIAL are defined,
you're able to compute your own shadows material passing the final 3D positions
to the vertex program, and depth parameters to the pixel program.
Simply use the cp3d::rendering::ICP3DMaterialCreator and add the
CP3D_COMPUTE_SHADOWS_MATERIAL define.
*/
#if defined(CP3D_MATERIAL) && defined(CP3D_COMPUTE_SHADOWS_MATERIAL)
/*
Let you create the shadows vertex function
*/
struct VS_INPUT_SHADOWS_MATERIAL
{
	float3 Normal : NORMAL;
};

struct VS_OUTPUT_SHADOWS_MATERIAL
{
	float4 ShadowMapSamplingPos : TEXCOORD0;
	float4 MVar        			: TEXCOORD1;
};

VS_OUTPUT_SHADOWS_MATERIAL computeShadowsVertex(float4 p, VS_INPUT_SHADOWS_MATERIAL In) {
	VS_OUTPUT_SHADOWS_MATERIAL OUT = (VS_OUTPUT_SHADOWS_MATERIAL)0;

	float4 SMPos = mul(float4(p.xyz, 1.0), mWorldViewProj2);
	SMPos.xy = float2(SMPos.x, -SMPos.y) / 2.0;
	OUT.ShadowMapSamplingPos = SMPos;

	#ifdef DIRECT3D_11
	float4 worldpos = mul(float4(In.Position.x, In.Position.y, In.Position.z, 1.0), mWorldTrans);
	float3 LightDir = normalize(LightPos - worldpos.xyz);
	#else
	float3 LightDir = normalize(LightPos - In.Position);
	#endif

	OUT.MVar = float4(SMPos.z, dot(In.Normal, LightDir), 1.0 / MAPRES, MaxD);

	return (OUT);
}

/*
Let you create the shadows pixel function
*/

#endif

#endif /// End __FX_UTILS_HLSL_FX_INCLUDED_
