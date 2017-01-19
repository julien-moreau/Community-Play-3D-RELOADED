#ifdef OPENGL_DRIVER

uniform float screenX, screenY;
uniform vec3 LineStarts0, LineStarts1, LineStarts2, LineStarts3;
uniform vec3 LineEnds0, LineEnds1, LineEnds2, LineEnds3;

void main() 
{
	gl_Position = vec4(gl_Vertex.x, gl_Vertex.y, 0.0, 1.0); 
	vec2 tCoords; 
	tCoords.x = 0.5 * (1.0 + gl_Vertex.x); 
	tCoords.y = 0.5 * (1.0 + gl_Vertex.y); 
	gl_TexCoord[0].xy = tCoords.xy; 
	tCoords.y = 1.0 - tCoords.y; 
	vec3 tLStart = mix(LineStarts0, LineStarts1, tCoords.x); 
	vec3 bLStart = mix(LineStarts2, LineStarts3, tCoords.x); 
	gl_TexCoord[1].xyz = mix(tLStart, bLStart, tCoords.y); 
	vec3 tLEnd = mix(LineEnds0, LineEnds1, tCoords.x); 
	vec3 bLEnd = mix(LineEnds2, LineEnds3, tCoords.x); 
	gl_TexCoord[2].xyz = mix(tLEnd, bLEnd, tCoords.y); 
	gl_TexCoord[3].xy = vec2(screenX, screenY); 
}

#else

float screenX, screenY; 
float3 LineStarts0, LineStarts1, LineStarts2, LineStarts3; 
float3 LineEnds0, LineEnds1, LineEnds2, LineEnds3; 

struct VS_OUTPUT
{
	float4 Position		: SV_Position;
	float2 TexCoords	: TEXCOORD0;
	float3 LStart		: TEXCOORD1;
	float3 LEnd			: TEXCOORD2;
	float2 ScreenSize	: TEXCOORD3;
};

struct VS_INPUT {
	float3 Position  : POSITION0;
	float2 TexCoords : TEXTURE0;
};

VS_OUTPUT vertexMain(VS_INPUT input)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	
	OUT.Position = float4(input.Position.x, input.Position.y, input.Position.z, 1.0);
	OUT.TexCoords.x = 0.5 * (1.0 + input.Position.x + (1.0 / screenX));
	OUT.TexCoords.y = 1.0 - 0.5 * (1.0 + input.Position.y - (1.0 / screenY));

	float3 tLStart = lerp(LineStarts0, LineStarts1, OUT.TexCoords.x);
	float3 bLStart = lerp(LineStarts2, LineStarts3, OUT.TexCoords.x);
	OUT.LStart = lerp(tLStart, bLStart, OUT.TexCoords.y);

	float3 tLEnd = lerp(LineEnds0, LineEnds1, OUT.TexCoords.x); 
	float3 bLEnd = lerp(LineEnds2, LineEnds3, OUT.TexCoords.x); 
	OUT.LEnd = lerp(tLEnd, bLEnd, OUT.TexCoords.y); 
	OUT.ScreenSize = float2(screenX, screenY);

	return (OUT);
}

#endif
