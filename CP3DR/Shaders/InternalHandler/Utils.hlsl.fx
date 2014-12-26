#ifdef OPENGL_DRIVER
#error "These util function are not intended for OpenGL Drivers"
#endif

#ifdef POST_PROCESS
struct VS_OUTPUT
{
	float4 Position		: SV_Position;
	float2 TexCoords	: TEXCOORD0;
	float3 LStart		: TEXCOORD1;
	float3 LEnd			: TEXCOORD2;
	float2 ScreenSize	: TEXCOORD3;
};
#endif

// Texture2D & sampler2D
inline float4 CP3DTex2D(sampler2D s, float2 t, SamplerState st) {
	return tex2D(s, t);
}

inline float4 CP3DTex2D(Texture2D s, float2 t, SamplerState st) {
	return s.Sample(st, t);
}
