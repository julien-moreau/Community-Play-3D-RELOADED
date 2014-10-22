#define SAMPLE_COUNT 8
#define SSAO_REACH 1.0
#define MAX_DEPTH_DECLINE 0.2
#define DEPTH_ALIASING_EPISILON 0.02
#define RAND_TEXTURE_TILES 3.0
#define SSAO_OUTPUT_MULTIPLIER 10000.5

##ifdef OPENGL_DRIVER

uniform sampler2D DepthMapSampler;
uniform sampler2D UserMapSampler;

uniform mat4 mViewProj;

float getDepthAt(vec2 coords)
{
	vec4 texDepth = texture2D(DepthMapSampler, coords);
	float extractedDepth = texDepth.r;
	return extractedDepth;
}

vec3 offsets[SAMPLE_COUNT];

void main()
{
	float currentDepth = getDepthAt(gl_TexCoord[0].xy);
	vec3 lVec = gl_TexCoord[2].xyz - gl_TexCoord[1].xyz;
	vec3 lDir = normalize(lVec);
	float lLength = length(lVec);
	vec3 currentWPos = gl_TexCoord[1].xyz + currentDepth * lDir * lLength;
	
	offsets[0] = vec3( 0.3524245,  0.2452452,  0.7425442);
	offsets[1] = vec3(-0.5242424, -0.4125105, -0.7867286);
	offsets[2] = vec3(-0.7857242,  0.2453452,  0.7242454);
	offsets[3] = vec3(-0.1524532, -0.9456342, -0.1452245);
	offsets[4] = vec3(-0.2535434,  0.4535424,  0.5444254);
	offsets[5] = vec3( 0.3135244, -0.4330132, -0.5422425);
	offsets[6] = vec3( 0.7897852, -0.2524242,  0.4584555);
	offsets[7] = vec3( 0.9792424,  0.0122242, -0.5422161);
	
	float totalOcclusion = 0.0;
	vec4 randCol = texture2D(UserMapSampler, gl_TexCoord[0].xy * RAND_TEXTURE_TILES);

	for(int i = 0;i < SAMPLE_COUNT;++i)
	{
		// Reflect the random offset using the random user texture.
		
		vec3 randVec = reflect(normalize(offsets[i]), randCol.xyz * 2.0 - vec3(1.0, 1.0, 1.0)) * SSAO_REACH;
		vec4 newWorldPos = vec4(currentWPos + randVec, 1.0);
		
		// Calculate our sample depth.
		float sampleDepth = length(newWorldPos.xyz - gl_TexCoord[1].xyz);
	
		// Project the position to the screen.
		newWorldPos = mViewProj * newWorldPos;
		newWorldPos.xy = vec2(newWorldPos.x, newWorldPos.y) / newWorldPos.w * 0.5 + vec2(0.5, 0.5);

		sampleDepth = sampleDepth / lLength - DEPTH_ALIASING_EPISILON;

		// Read from the projected position.
		float newDepth = getDepthAt(clamp(newWorldPos.xy, vec2(0.001, 0.001), vec2(0.999, 0.999)));

		// Compare the two depth samples.
		float depthCalc = newDepth < sampleDepth ? 1.0 : 0.0;
		depthCalc *= clamp(1.0 - (sampleDepth - newDepth) / MAX_DEPTH_DECLINE, 0.0, 1.0);
		totalOcclusion += depthCalc;
	}
	
	totalOcclusion /= float(SAMPLE_COUNT);
	
	gl_FragColor = vec4(1.0 - totalOcclusion * SSAO_OUTPUT_MULTIPLIER);
}

##else

sampler2D DepthMapSampler : register(s2);
sampler2D UserMapSampler : register(s3);

float4x4 mViewProj;

float getDepthAt(float2 coords)
{
	float4 texDepth = tex2D(DepthMapSampler, coords);
	float extractedDepth = texDepth.r;
	return extractedDepth;
}

float4 pixelMain
(
	float2 TexCoords : TEXCOORD0,
	float3 LStart : TEXCOORD1,
	float3 LEnd : TEXCOORD2
) : COLOR0
{
	float currentDepth = getDepthAt(TexCoords);
	float3 lVec = LEnd - LStart;
	float3 lDir = normalize(lVec);
	float lLength = length(lVec);
	float3 currentWPos = LStart + currentDepth * lDir * lLength;
	
	const float3 offsets[SAMPLE_COUNT] =
	{
		float3( 0.3524245,  0.2452452,  0.7425442),
		float3(-0.5242424, -0.4125105, -0.7867286),
		float3(-0.7857242,  0.2453452,  0.7242454),
		float3(-0.1524532, -0.9456342, -0.1452245),
		float3(-0.2535434,  0.4535424,  0.5444254),
		float3( 0.3135244, -0.4330132, -0.5422425),
		float3( 0.7897852, -0.2524242,  0.4584555),
		float3( 0.9792424,  0.0122242, -0.5422161)
	};
	
	float totalOcclusion = 0.0;
	float4 randCol = tex2D(UserMapSampler, TexCoords * RAND_TEXTURE_TILES);

	for(int i = 0;i < SAMPLE_COUNT;++i)
	{
		// Reflect the random offset using the random user texture.
		float3 randVec = reflect(normalize(offsets[i]), randCol.xyz * 2.0 - float3(1.0, 1.0, 1.0)) * SSAO_REACH;
		float4 newWorldPos = float4(currentWPos + randVec, 1.0);
		
		// Calculate our sample depth.
		float sampleDepth = (length(newWorldPos.xyz - LStart) / lLength) - DEPTH_ALIASING_EPISILON;
	
		// Project the position to the screen.
		newWorldPos = mul(newWorldPos, mViewProj);
		newWorldPos.xy = float2(newWorldPos.x, -newWorldPos.y) / newWorldPos.w * 0.5 + float2(0.5, 0.5);

		// Read from the projected position.
		float newDepth = getDepthAt(clamp(newWorldPos.xy, float2(0.001, 0.001), float2(0.999, 0.999)));

		// Compare the two depth samples.
		float depthCalc = newDepth < sampleDepth ? 1.0 : 0.0;
		depthCalc *= clamp(1.0 - (sampleDepth - newDepth) / MAX_DEPTH_DECLINE, 0.0, 1.0);
		totalOcclusion += depthCalc;
	}
	
	totalOcclusion /= SAMPLE_COUNT;
	
	return 1.0 - totalOcclusion * SSAO_OUTPUT_MULTIPLIER;
}

##endif
