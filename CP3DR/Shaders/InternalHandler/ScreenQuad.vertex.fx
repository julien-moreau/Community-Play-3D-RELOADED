float screenX, screenY; 
float3 LineStarts0, LineStarts1, LineStarts2, LineStarts3; 
float3 LineEnds0, LineEnds1, LineEnds2, LineEnds3; 

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float2 TexCoords	: TEXCOORD0;
	float3 LStart		: TEXCOORD1;
	float3 LEnd			: TEXCOORD2;
	float2 ScreenSize	: TEXCOORD3;
};

VS_OUTPUT vertexMain(float3 Position : POSITION, float4 TexCoords : TEXTURE0)
{
	VS_OUTPUT OUT;
	
	OUT.Position = float4(Position.x, Position.y, Position.z, 1.0);
	OUT.TexCoords.x = 0.5 * (1.0 + Position.x + (1.0 / screenX));
	OUT.TexCoords.y = 1.0 - 0.5 * (1.0 + Position.y - (1.0 / screenY));

	float3 tLStart = lerp(LineStarts0, LineStarts1, OUT.TexCoords.x);
	float3 bLStart = lerp(LineStarts2, LineStarts3, OUT.TexCoords.x);
	OUT.LStart = lerp(tLStart, bLStart, OUT.TexCoords.y);

	float3 tLEnd = lerp(LineEnds0, LineEnds1, OUT.TexCoords.x); 
	float3 bLEnd = lerp(LineEnds2, LineEnds3, OUT.TexCoords.x); 
	OUT.LEnd = lerp(tLEnd, bLEnd, OUT.TexCoords.y); 
	OUT.ScreenSize = float2(screenX, screenY);

	return (OUT);
}

Texture2D ColorMapSampler  : register(t0);
Texture2D ScreenMapSampler : register(t1);

SamplerState ColorMapSamplerST : register(s0);
SamplerState ScreenMapSamplerST : register(s1);

float4 pixelMain(VS_OUTPUT In) : SV_TARGET
{
	float4 finalCol = ColorMapSampler.Sample(ColorMapSamplerST, In.TexCoords);
	float4 lightCol = ScreenMapSampler.Sample(ScreenMapSamplerST, In.TexCoords);

	return finalCol * lightCol;
}

