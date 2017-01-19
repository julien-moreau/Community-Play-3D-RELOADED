#ifdef OPENGL_DRIVER

uniform sampler2D ShadowMapSampler;
uniform vec4 LightColour;

varying float lightVal;

#ifdef VSM
float testShadow(vec2 texCoords, vec2 offset, float RealDist)
{
	vec4 shadTexCol = texture2D(ShadowMapSampler, texCoords + offset);

	float lit_factor = (RealDist <= shadTexCol.x) ? 1.0 : 0.0;

	float E_x2 = shadTexCol.y;
	float Ex_2 = shadTexCol.x * shadTexCol.x;
	float variance = min(max(E_x2 - Ex_2, 0.00001) + 0.000001, 1.0);
	float m_d = (shadTexCol.x - RealDist);
	float p = variance / (variance + m_d * m_d);

	return (1.0 - max(lit_factor, p)) / float(SAMPLE_AMOUNT);
}
#else
float testShadow(vec2 smTexCoord, vec2 offset, float realDistance)
{
	vec4 texDepth = texture2D(ShadowMapSampler, vec2( smTexCoord + offset));
	float extractedDistance = texDepth.r;
	
	return (extractedDistance <= realDistance) ? (1.0  / float(SAMPLE_AMOUNT)) : 0.0;
}
#endif

vec2 offsetArray[16];

void main() 
{
	vec4 SMPos = gl_TexCoord[0];
	vec4 MVar = gl_TexCoord[1];

	offsetArray[0] = vec2(0.0, 0.0);
	offsetArray[1] = vec2(0.0, 1.0);
	offsetArray[2] = vec2(1.0, 1.0);
	offsetArray[3] = vec2(-1.0, -1.0);
	offsetArray[4] = vec2(-2.0, 0.0);
	offsetArray[5] = vec2(0.0, -2.0);
	offsetArray[6] = vec2(2.0, -2.0);
	offsetArray[7] = vec2(-2.0, 2.0);
	offsetArray[8] = vec2(3.0, 0.0);
	offsetArray[9] = vec2(0.0, 3.0);
	offsetArray[10] = vec2(3.0, 3.0);
	offsetArray[11] = vec2(-3.0, -3.0);
	offsetArray[12] = vec2(-4.0, 0.0);
	offsetArray[13] = vec2(0.0, -4.0);
	offsetArray[14] = vec2(4.0, -4.0);
	offsetArray[15] = vec2(-4.0, 4.0);

    SMPos.xy  = SMPos.xy / SMPos.w / 2.0 + vec2(0.5, 0.5);

	vec4 finalCol = vec4(0.0, 0.0, 0.0, 0.0);

	// If this point is within the light's frustum.
	#ifdef ROUND_SPOTLIGHTS
	float lengthToCenter = length(SMPos.xy - vec2(0.5, 0.5));
	if(SMPos.z - 0.01 > 0.0 && SMPos.z + 0.01 < MVar.z)
	#else
	vec2 clampedSMPos = clamp(SMPos.xy, vec2(0.0, 0.0), vec2(1.0, 1.0));
	if(clampedSMPos.x == SMPos.x && clampedSMPos.y == SMPos.y && SMPos.z > 0.0 && SMPos.z < MVar.z)
	#endif
	{
		float lightFactor = 1.0;
		float realDist = MVar.x / MVar.z - 0.002;
	
		for(int i = 0;i < SAMPLE_AMOUNT; i++)
			lightFactor -= testShadow(SMPos.xy, offsetArray[i] * MVar.w, realDist);

		// Multiply with diffuse.
		#ifdef ROUND_SPOTLIGHTS
		finalCol = LightColour * lightFactor * MVar.y * clamp(5.0 - 10.0 * lengthToCenter, 0.0, 1.0);
		#else
		finalCol = LightColour * lightFactor * MVar.y;
		#endif
	}

	#ifdef ROUND_SPOTLIGHTS
	gl_FragColor = finalCol;
	#else
	else
	{
		finalCol = LightColour * MVar.y;
	}
	gl_FragColor = finalCol;
	#endif
}

#else

#include "Shaders/InternalHandler/Utils.hlsl.fx"

/*
CP3DTexture ShadowMapSampler : registerTexture(t0);
SamplerState ShadowMapSamplerST : register(s0);
*/

SHADOW_MAP_SAMPLER0
SHADOW_MAP_SAMPLER1
SHADOW_MAP_SAMPLER2
SHADOW_MAP_SAMPLER3
SHADOW_MAP_SAMPLER4
SHADOW_MAP_SAMPLER5

float4 LightColour;

struct VS_OUTPUT
{
	float4 Position				: SV_Position;
	float4 ShadowMapSamplingPos : TEXCOORD0;
	float4 MVar        			: TEXCOORD1;
};

#ifdef VSM
float calcShadow(float2 texCoords, float2 offset, float RealDist, CP3DTexture shadowMapSampler, SamplerState shadowMapSamplerST)
{
	float4 shadTexCol = CP3DTex2D(shadowMapSampler, texCoords + offset, shadowMapSamplerST);

	float lit_factor = (RealDist <= shadTexCol.r);

	float E_x2 = shadTexCol.g;
	float Ex_2 = shadTexCol.r * shadTexCol.r;
	float variance = min(max(E_x2 - Ex_2, 0.00001) + 0.000001, 1.0);
	float m_d = (shadTexCol.r - RealDist);
	float p = variance / (variance + m_d * m_d);
	  
	return (1.0 - max(lit_factor, p)) / SAMPLE_AMOUNT;
}
#else
float calcShadow(float2 texCoords, float2 offset, float RealDist, CP3DTexture shadowMapSampler, SamplerState shadowMapSamplerST)
{
	float4 shadTexCol = CP3DTex2D(shadowMapSampler, texCoords + offset, shadowMapSamplerST);

	float extractedDistance = shadTexCol.r;
      
   return (extractedDistance <= RealDist ? (1.0  / SAMPLE_AMOUNT) : 0.0);
}
#endif

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
	#ifdef ROUND_SPOTLIGHTS
	float lengthToCenter = length(SMPos.xy - float2(0.5, 0.5));
	if (lengthToCenter < 0.5 && SMPos.z > 0.0 && SMPos.z < In.MVar[3])
	#else
	float2 clampedSMPos = saturate(SMPos.xy);
	if (clampedSMPos.x == SMPos.x && clampedSMPos.y == SMPos.y && SMPos.z > 0.0 && SMPos.z < In.MVar[3])
	#endif
	{
		float lightFactor = 1.0;
		float realDistance = In.MVar[0] / In.MVar[3] - 0.005;
	
		[unroll]
		for(unsigned int i = 0; i < SAMPLE_AMOUNT; ++i)
			lightFactor -= calcShadow(SMPos.xy, offsetArray[i] * In.MVar[2], realDistance, ShadowMapSampler0, ShadowMapSampler0ST);

		// Multiply with diffuse.
		#ifdef ROUND_SPOTLIGHTS
		finalCol = LightColour * lightFactor * In.MVar[1] * clamp(5.0 - 10.0 * lengthToCenter, 0.0, 1.0);
		#else
		finalCol = LightColour * lightFactor * In.MVar[1];
		#endif
	}
	
	#ifdef ROUND_SPOTLIGHTS
	return finalCol;
	#else
	else
	{
		finalCol = LightColour * In.MVar[1];
	}
	return finalCol;
	#endif
}

#endif
