
#ifdef OPENGL_DRIVER

uniform int   numLights;
uniform vec3  fvLightPosition[__CP3D__MAX_LIGHTS__];
uniform float fLightStrength[__CP3D__MAX_LIGHTS__];
uniform mat4  matWorldInverse;

varying vec2  Texcoord;
varying vec3  ViewDirection;
varying vec3  LightDirection[__CP3D__MAX_LIGHTS__];
varying float LightDistMultiplier[__CP3D__MAX_LIGHTS__];

float getLengthSQR (vec3 vec)
{
    return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
}

void main( void )
{
	mat4 LightTransform = gl_ModelViewMatrix;
    LightTransform = LightTransform*matWorldInverse;
    
    gl_Position = ftransform();
    Texcoord    = gl_MultiTexCoord0.xy;
    
    vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;
    vec3 fvViewDirection  = -fvObjectPosition.xyz;
    
    vec3 fvNormal    = gl_NormalMatrix * gl_Normal;
    vec3 fvTangent   = -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0);
    vec3 fvBinormal  = cross(fvTangent,gl_Normal);
    fvTangent=gl_NormalMatrix*cross(fvBinormal,gl_Normal);
    fvBinormal=gl_NormalMatrix*fvBinormal;
    
    for (int i=0; i < numLights; i++) {
        vec4 fvLightPos = LightTransform * vec4(fvLightPosition[i],1.0);
        vec3 fvLightDirection = (fvLightPos.xyz - fvObjectPosition.xyz);
        LightDistMultiplier[i] = 1.0 / (getLengthSQR (fvLightDirection) / (fLightStrength[i] * 10000.0));
        
        LightDirection[i].x  = dot( fvTangent, fvLightDirection.xyz );
        LightDirection[i].y  = dot( fvBinormal, fvLightDirection.xyz );
        LightDirection[i].z  = dot( fvNormal, fvLightDirection.xyz );
    }
    
    ViewDirection.x  = dot( fvTangent, fvViewDirection );
    ViewDirection.y  = dot( fvBinormal, fvViewDirection );
    ViewDirection.z  = dot( fvNormal, fvViewDirection );
}

#else

cbuffer cbParams : register(c0)
{
	float4x4 ModelViewProjectionMatrix;
}

struct VS_OUTPUT
{
   float4 Position            : SV_Position;
   float2 Texcoord            : TEXCOORD0;
   float3 Normal			  : TEXCOORD1;
   float4 ObjectPosition	  : TEXCOORD2;
};

struct VS_INPUT
{
	float3 Position         : POSITION;
    float3 Normal           : NORMAL;
    float2 Texcoord         : TEXCOORD0;
};

VS_OUTPUT vertexMain( in VS_INPUT IN ) 
{    
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = mul(float4(IN.Position.x, IN.Position.y, IN.Position.z, 1.0), ModelViewProjectionMatrix);
	OUT.Texcoord = IN.Texcoord;
	OUT.Normal   = IN.Normal;
	OUT.ObjectPosition = OUT.Position;

	return OUT;
}


#endif
