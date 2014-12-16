##ifdef DIRECT3D_11
	Texture2D ShadowMapSampler : register(t0);
	SamplerState ShadowMapSamplerST : register(s0);
##else
	sampler2D ShadowMapSampler : register(s0);
##endif

float4 LightColour;

struct VS_OUTPUT
{
	float4 Position				: SV_Position;
	float4 ShadowMapSamplingPos : TEXCOORD0;
	float4 MVar        			: TEXCOORD1;
	float2 TexCoords            : TEXCOORD2;
};

##ifdef VSM
float calcShadow(float2 texCoords, float2 offset, float RealDist)
{
##ifdef DIRECT3D_11
	float4 shadTexCol = ShadowMapSampler.Sample(ShadowMapSamplerST, texCoords + offset);
##else
	float4 shadTexCol = tex2D(ShadowMapSampler, texCoords + offset);
##endif

	float lit_factor = (RealDist <= shadTexCol.r);

	float E_x2 = shadTexCol.g;
	float Ex_2 = shadTexCol.r * shadTexCol.r;
	float variance = min(max(E_x2 - Ex_2, 0.00001) + 0.000001, 1.0);
	float m_d = (shadTexCol.r - RealDist);
	float p = variance / (variance + m_d * m_d);
	  
	return (1.0 - max(lit_factor, p)) / SAMPLE_AMOUNT;
}
##else
float calcShadow(float2 texCoords, float2 offset, float RealDist)
{
##ifdef DIRECT3D_11
	float4 shadTexCol = ShadowMapSampler.Sample(ShadowMapSamplerST, texCoords + offset);
##else
	float4 shadTexCol = tex2D(ShadowMapSampler, texCoords + offset);
##endif

	float extractedDistance = shadTexCol.r;
      
   return (extractedDistance <= RealDist ? (1.0  / SAMPLE_AMOUNT) : 0.0);
}
##endif

const float2 offsetArray[16] =
{
	float2(0.0, 0.0),
	float2(0.0, 1.0),
	float2(1.0, -1.0),
	float2(-1.0, 1.0),
	float2(-2.0, 0.0),
	float2(0.0, -2.0),
	float2(-2.0, -2.0),
	float2(2.0, 2.0),
	float2(3.0, 0.0),
	float2(0.0, 3.0),
	float2(3.0, -3.0),
	float2(-3.0, 3.0),
	float2(-4.0, 0.0),
	float2(0.0, -4.0),
	float2(-4.0, -4.0),
	float2(4.0, 4.0)
};

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float4 SMPos = In.ShadowMapSamplingPos;
	SMPos.xy = In.ShadowMapSamplingPos.xy / In.ShadowMapSamplingPos.w + float2(0.5, 0.5);

	float4 finalCol = float4(0.0, 0.0, 0.0, 0.0);

	// If this point is within the light's frustum.
##ifdef ROUND_SPOTLIGHTS
	float lengthToCenter = length(SMPos.xy - float2(0.5, 0.5));
	if (lengthToCenter < 0.5 && SMPos.z > 0.0 && SMPos.z < In.MVar[3])
##else
	float2 clampedSMPos = saturate(SMPos.xy);
	if (clampedSMPos.x == SMPos.x && clampedSMPos.y == SMPos.y && SMPos.z > 0.0 && SMPos.z < In.MVar[3])
##endif
	{
		float lightFactor = 1.0;
		float realDistance = In.MVar[0] / In.MVar[3] - 0.005;
	
		for(unsigned int i = 0; i < SAMPLE_AMOUNT; ++i)
			lightFactor -= calcShadow(SMPos.xy, offsetArray[i] * In.MVar[2], realDistance);

		// Multiply with diffuse.
##ifdef ROUND_SPOTLIGHTS
		finalCol = LightColour * lightFactor * In.MVar[1] * clamp(5.0 - 10.0 * lengthToCenter, 0.0, 1.0);
##else
		finalCol = LightColour * lightFactor * In.MVar[1];
##endif
	}
	
##ifdef ROUND_SPOTLIGHTS
	return finalCol;
##else
	else
	{
		finalCol = LightColour * In.MVar[1];
	}
	return finalCol;
##endif
}