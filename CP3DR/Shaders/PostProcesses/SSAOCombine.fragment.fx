##ifdef OPENGL_DRIVER

uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

void main() 
{		
	vec4 screenCol = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);
	vec4 SSAOCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);

	gl_FragColor = (screenCol * SSAOCol);
}

##else

sampler2D ColorMapSampler : register(s0);
sampler2D ScreenMapSampler : register(s1);

float4 pixelMain(float2 TexCoords : TEXCOORD0) : COLOR0
{		
	float4 screenCol = tex2D(ScreenMapSampler, TexCoords);
	float4 SSAOCol = tex2D(ColorMapSampler, TexCoords);

	return(screenCol * SSAOCol);
}

##endif
