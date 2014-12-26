#ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

void main() 
{		
	vec4 screenCol = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);
	vec4 SSAOCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);

	gl_FragColor = (screenCol * SSAOCol);
}

#else

#ifdef DIRECT3D_11
Texture2D ColorMapSampler : register(t0);
Texture2D ScreenMapSampler : register(t1);

SamplerState ColorMapSamplerST : register(s0);
SamplerState ScreenMapSamplerST : register(s1);
#else
sampler2D ColorMapSampler : register(s0);
sampler2D ScreenMapSampler : register(s1);
#endif

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	#ifdef DIRECT3D_11
	float4 screenCol = ScreenMapSampler.Sample(ScreenMapSamplerST, In.TexCoords.xy);
	float4 SSAOCol = ColorMapSampler.Sample(ColorMapSamplerST, In.TexCoords.xy);
	#else
	float4 screenCol = tex2D(ScreenMapSampler, In.TexCoords.xy);
	float4 SSAOCol = tex2D(ColorMapSampler, In.TexCoords.xy);
	#endif

	return(screenCol * SSAOCol);
}

#endif
