
#ifdef OPENGL_DRIVER

uniform mat4 mWorldViewProj;

void main(void) {
	vec4 tPos = mWorldViewProj * gl_Vertex;
	gl_Position = tPos;
	gl_TexCoord[0] = tPos;
}

#else

float4x4 mWorldViewProj;

struct VS_OUTPUT 
{
	float4 Position : POSITION0;
	float4 TexCoord : TEXCOORD0;
};

VS_OUTPUT vertexMain(float3 Position : POSITION0)
{
	VS_OUTPUT  OUT;
	OUT.Position = mul(float4(Position.x, Position.y, Position.z, 1.0), mWorldViewProj);
	OUT.TexCoord = OUT.Position;
	return (OUT);
}

#endif
