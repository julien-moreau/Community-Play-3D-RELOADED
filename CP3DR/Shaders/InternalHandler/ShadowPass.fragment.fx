#ifdef OPENGL_DRIVER

uniform sampler2D ShadowMapSampler0, ShadowMapSampler1, ShadowMapSampler2, ShadowMapSampler3, ShadowMapSampler4, ShadowMapSampler5;
uniform vec4 LightColour0, LightColour1, LightColour2, LightColour3, LightColour4, LightColour5;

varying vec4 SMPos0, SMPos1, SMPos2, SMPos3, SMPos4, SMPos5;
varying vec4 MVar0, MVar1, MVar2, MVar3, MVar4, MVar5;

#ifdef VSM
float testShadow(in vec2 texCoords, in vec2 offset, in float RealDist, in sampler2D shadowMapSampler)
{
	vec4 shadTexCol = texture2D(shadowMapSampler, texCoords + offset);

	float lit_factor = (RealDist <= shadTexCol.x) ? 1.0 : 0.0;
    
	float E_x2 = shadTexCol.y;
	float Ex_2 = shadTexCol.x * shadTexCol.x;
	float variance = min(max(E_x2 - Ex_2, 0.00001) + 0.000001, 1.0);
	float m_d = (shadTexCol.x - RealDist);
	float p = variance / (variance + m_d * m_d);

	return (1.0 - max(lit_factor, p)) / float(SAMPLE_AMOUNT);
}
#else
float testShadow(in vec2 smTexCoord, in vec2 offset, in float realDistance, in sampler2D shadowMapSampler)
{
	vec4 texDepth = texture2D(shadowMapSampler, vec2( smTexCoord + offset));
	float extractedDistance = texDepth.r;
	
	return (extractedDistance <= realDistance) ? (1.0  / float(SAMPLE_AMOUNT)) : 0.0;
}
#endif

vec4 computeShadows(in vec4 SMPos, in vec4 MVar, in vec4 lightColour, in sampler2D shadowMapSampler)
{
    vec2 offsetArray[16];
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
            lightFactor -= testShadow(SMPos.xy, offsetArray[i] * MVar.w, realDist, shadowMapSampler);
        
        // Multiply with diffuse.
        #ifdef ROUND_SPOTLIGHTS
        finalCol = lightColour * lightFactor * MVar.y * clamp(5.0 - 10.0 * lengthToCenter, 0.0, 1.0);
        #else
        finalCol = lightColour * lightFactor * MVar.y;
        #endif
    }
    
    #ifdef ROUND_SPOTLIGHTS
    return finalCol;
    #else
    else
    {
        finalCol = lightColour * MVar.y;
    }
    return finalCol;
    #endif
}

void main()
{
    gl_FragColor = computeShadows(SMPos0, MVar0, LightColour0, ShadowMapSampler0);
    
    #if (LIGHTS_COUNT >= 2)
    gl_FragColor += computeShadows(SMPos1, MVar1, LightColour1, ShadowMapSampler1);
    #endif
    
    #if (LIGHTS_COUNT >= 3)
    gl_FragColor += computeShadows(SMPos2, MVar2, LightColour2, ShadowMapSampler2);
    #endif
    
    #if (LIGHTS_COUNT >= 4)
    gl_FragColor += computeShadows(SMPos3, MVar3, LightColour3, ShadowMapSampler3);
    #endif
    
    #if (LIGHTS_COUNT >= 5)
    gl_FragColor += computeShadows(SMPos4, MVar4, LightColour4, ShadowMapSampler4);
    #endif
    
    #if (LIGHTS_COUNT >= 6)
    gl_FragColor += computeShadows(SMPos5, MVar5, LightColour5, ShadowMapSampler5);
    #endif
    
    gl_FragColor /= float(LIGHTS_COUNT);
}

#else

#include "Shaders/InternalHandler/Utils.hlsl.fx"

CP3DTexture ShadowMapSampler0 : registerTexture(t0);
SamplerState ShadowMapSamplerST0 : register(s0);

CP3DTexture ShadowMapSampler1 : registerTexture(t1);
SamplerState ShadowMapSamplerST1 : register(s1);

CP3DTexture ShadowMapSampler2 : registerTexture(t2);
SamplerState ShadowMapSamplerST2 : register(s2);

CP3DTexture ShadowMapSampler3 : registerTexture(t3);
SamplerState ShadowMapSamplerST3 : register(s3);

CP3DTexture ShadowMapSampler4 : registerTexture(t4);
SamplerState ShadowMapSamplerST4 : register(s4);

CP3DTexture ShadowMapSampler5 : registerTexture(t5);
SamplerState ShadowMapSamplerST5 : register(s5);

float4 LightColour0, LightColour1, LightColour2, LightColour3, LightColour4, LightColour5;

struct VS_OUTPUT
{
	float4 Position				 : SV_Position;

	float4 ShadowMapSamplingPos0 : TEXCOORD0;
	float4 MVar0        		 : TEXCOORD1;

	#if (LIGHTS_COUNT >= 2)
	float4 ShadowMapSamplingPos1 : TEXCOORD2;
	float4 MVar1        		 : TEXCOORD3;
	#endif

	#if (LIGHTS_COUNT >= 3)
	float4 ShadowMapSamplingPos2 : TEXCOORD4;
	float4 MVar2        		 : TEXCOORD5;
	#endif

	#if (LIGHTS_COUNT >= 4)
	float4 ShadowMapSamplingPos3 : TEXCOORD6;
	float4 MVar3        		 : TEXCOORD7;
	#endif
};

#ifdef VSM
inline float calcShadow(float2 texCoords, float2 offset, float RealDist, CP3DTexture shadowMapSampler, SamplerState shadowMapSamplerST)
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
inline float calcShadow(float2 texCoords, float2 offset, float RealDist, CP3DTexture shadowMapSampler, SamplerState shadowMapSamplerST)
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

inline float4 getLightOutput(float4 ShadowMapSamplingPos, float4 MVar, float4 LightColour, sampler2D ShadowMapSampler, SamplerState ShadowMapSamplerST)
{
	float4 SMPos = ShadowMapSamplingPos;
	SMPos.xy = ShadowMapSamplingPos.xy / ShadowMapSamplingPos.w + float2(0.5, 0.5);

	float4 finalCol = float4(0.0, 0.0, 0.0, 0.0);

	// If this point is within the light's frustum.
	#ifdef ROUND_SPOTLIGHTS
	float lengthToCenter = length(SMPos.xy - float2(0.5, 0.5));
	if (lengthToCenter < 0.5 && SMPos.z > 0.0 && SMPos.z < MVar[3])
	#else
	float2 clampedSMPos = saturate(SMPos.xy);
	if (clampedSMPos.x == SMPos.x && clampedSMPos.y == SMPos.y && SMPos.z > 0.0 && SMPos.z < MVar[3])
	#endif
	{
		float lightFactor = 1.0;
		float realDistance = MVar[0] / MVar[3] - 0.005;
	
		[unroll]
		for(unsigned int i = 0; i < SAMPLE_AMOUNT; ++i)
			lightFactor -= calcShadow(SMPos.xy, offsetArray[i] * MVar[2], realDistance, ShadowMapSampler, ShadowMapSamplerST);

		// Multiply with diffuse.
		#ifdef ROUND_SPOTLIGHTS
		finalCol = LightColour * lightFactor * MVar[1] * clamp(5.0 - 10.0 * lengthToCenter, 0.0, 1.0);
		#else
		finalCol = LightColour * lightFactor * MVar[1];
		#endif
	}
	
	#ifdef ROUND_SPOTLIGHTS
	return finalCol;
	#else
	else
	{
		finalCol = LightColour * MVar[1];
	}
	return finalCol;
	#endif
}

float4 pixelMain(VS_OUTPUT In) : COLOR0
{
	float4 finalCol = getLightOutput(In.ShadowMapSamplingPos0, In.MVar0, LightColour0, ShadowMapSampler0, ShadowMapSamplerST0);

	#if (LIGHTS_COUNT >= 2)
	finalCol += getLightOutput(In.ShadowMapSamplingPos1, In.MVar1, LightColour1, ShadowMapSampler1, ShadowMapSamplerST1);
    #endif

	#if (LIGHTS_COUNT >= 3)
	finalCol += getLightOutput(In.ShadowMapSamplingPos2, In.MVar2, LightColour2, ShadowMapSampler2, ShadowMapSamplerST2);
    #endif

	#if (LIGHTS_COUNT >= 4)
	finalCol += getLightOutput(In.ShadowMapSamplingPos3, In.MVar3, LightColour3, ShadowMapSampler3, ShadowMapSamplerST3);
    #endif

	return finalCol;
}

#endif
