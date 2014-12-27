
#ifdef OPENGL_DRIVER

uniform int  numLights;
uniform vec4 fvAmbient;
uniform vec4 fvLightColor[__CP3D__MAX_LIGHTS__];

uniform float fSpecularPower;
uniform float fSpecularStrength;
uniform float fBumpStrength;
uniform float shininess;

uniform sampler2D baseMap;
uniform sampler2D bumpMap;
uniform sampler2D specularMap;

varying vec2  Texcoord;
varying vec3  ViewDirection;
varying vec3  LightDirection[__CP3D__MAX_LIGHTS__];
varying float LightDistMultiplier[__CP3D__MAX_LIGHTS__];

void main( void )
{
	if (numLights == 0) {
		gl_FragColor = texture2D(baseMap, Texcoord);
	}
	else {
		vec4  fvBaseColor      = texture2D( baseMap, Texcoord );
		vec3  fvNormal         = texture2D( bumpMap, Texcoord ).yxz;
		vec4  fvTotalAmbient   = fvAmbient * fvBaseColor;
		fvNormal.xy *= 2.0;
		fvNormal.xy -= 1.0;
		fvNormal = (vec3(0.0,0.0,1.0) - fvNormal) * fBumpStrength + fvNormal;
		fvNormal = normalize(fvNormal);
		vec3  fvViewDirection = normalize( ViewDirection );
    
		vec4 fvTotalDiffuse  = vec4(0.0, 0.0, 0.0, 0.0);
		vec4 fvTotalSpecular = vec4(0.0, 0.0, 0.0, 0.0);
    
		for (int i=0; i < numLights; i++) {
			vec3  fvLightDirection = normalize( LightDirection[i] );
			float fNDotL           = max(dot(fvNormal, fvLightDirection),0.0)-0.1;
			vec3  fvReflection     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection );
			float fRDotV           = max( 0.0, dot( fvReflection, fvViewDirection ) );
			fvTotalDiffuse  += fvLightColor[i] * fNDotL * fvBaseColor * LightDistMultiplier[i];
			fvTotalSpecular += fNDotL * fvLightColor[i] * ( pow( fRDotV, fSpecularPower ) ) * LightDistMultiplier[i];
		}
    
		fvTotalSpecular *= fvTotalSpecular + texture2D(specularMap, Texcoord) * shininess;
		vec4 color = ( fvTotalAmbient + fvTotalDiffuse + (fvTotalSpecular * fSpecularStrength));

		if(color.r>1.0) { color.gb += color.r - 1.0; }
		if(color.g>1.0) { color.rb += color.g - 1.0; }
		if(color.b>1.0) { color.rg += color.b - 1.0; }

		gl_FragColor = color;
	}
}

#else

float4x4 matWorldInverse;
float4x4 ModelViewMatrix;
float3 fvLightPosition[__CP3D__MAX_LIGHTS__];
float fLightStrength[__CP3D__MAX_LIGHTS__];
float4 fvAmbient;
float4 fvLightColor[__CP3D__MAX_LIGHTS__];
float fSpecularPower;
float fSpecularStrength;
float fBumpStrength;
float shininess;
int numLights;

#ifdef DIRECT3D_11
Texture2D baseMap	  : register(t0);
Texture2D bumpMap	  : register(t1);
Texture2D specularMap : register(t2);

SamplerState baseMapST	   : register(s0);
SamplerState bumpMapST	   : register(s1);
SamplerState specularMapST : register(s2);
#else
sampler2D baseMap      : register(s0);
sampler2D bumpMap      : register(s1);
sampler2D specularMap  : register(s2);
#endif

struct VS_OUTPUT
{
	float4 Position           : SV_Position;
	float2 Texcoord           : TEXCOORD0;
	float3 Normal			  : TEXCOORD1;
	float4 ObjectPosition	  : TEXCOORD2;
};

float getLengthSQR (float3 vec) 
{ 
   return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z); 
}

float4 pixelMain(in VS_OUTPUT IN) : COLOR0
{
	if (numLights == 0) {
		#ifdef DIRECT3D_11
		return baseMap.Sample(baseMapST, IN.Texcoord.xy);
		#else
		return tex2D( baseMap, IN.Texcoord );
		#endif
	}
	else {
		/// Added vertex
		float4x4 LightTransform= ModelViewMatrix;
		LightTransform= mul(matWorldInverse, LightTransform);
		float4 fvObjectPosition = IN.ObjectPosition;

		float3 fvTangent   = -float3(abs(IN.Normal.y) + abs(IN.Normal.z), abs(IN.Normal.x), 0.0); 
		float3 fvBinormal  = cross(fvTangent, IN.Normal);
		float3 fvNormal    = mul(float4(IN.Normal.xyz, 0.0), ModelViewMatrix).xyz;
		fvTangent          = mul(float4(cross(fvBinormal.xyz, IN.Normal.xyz), 0.0), ModelViewMatrix).xyz;
		fvBinormal         = mul(float4(fvBinormal.xyz, 0.0), ModelViewMatrix).xyz;

		float3 fvViewDirection  = -fvObjectPosition.xyz; 
		float3 ViewDirection	= float3(0.0, 0.0, 0.0);
		ViewDirection.x			= dot(fvTangent, fvViewDirection); 
		ViewDirection.y			= dot(fvBinormal, fvViewDirection); 
		ViewDirection.z			= dot(fvNormal, fvViewDirection); 

		/// End added vertex

		float4 color	 = float4(0.0, 0.0, 0.0, 0.0);
#ifdef DIRECT3D_11
		float3 fvNormal2 = bumpMap.Sample(bumpMapST, IN.Texcoord.xy).yxz;
#else
		float3 fvNormal2 = tex2D(bumpMap, IN.Texcoord).yxz;
#endif
   
		fvNormal2.xy *= 2.0; 
		fvNormal2.xy -= 1.0;
		fvNormal2 = (float3(0.0, 0.0, 1.0) - fvNormal2) * fBumpStrength + fvNormal2; 
		fvNormal2 = normalize(fvNormal2);
   
#ifdef DIRECT3D_11
		float4  fvBaseColor = baseMap.Sample(baseMapST, IN.Texcoord.xy);
#else
		float4  fvBaseColor      = tex2D( baseMap, IN.Texcoord );
#endif
		float4  fvTotalAmbient   = fvAmbient * fvBaseColor;
		float4  fvTotalDiffuse   = float4(0.0, 0.0, 0.0, 0.0);
		float4  fvTotalSpecular  = float4(0.0, 0.0, 0.0, 0.0);

		for (int i=0; i < numLights; i++) {
			/// Added vertex
			float4 fvLightPos1 = mul(float4(fvLightPosition[i].xyz, 1.0), LightTransform);
			float3 fvLightDirection1 = (fvLightPos1.xyz - fvObjectPosition.xyz);

			float3 LightDirection1;
			LightDirection1.x  = dot(fvTangent, fvLightDirection1.xyz); 
			LightDirection1.y  = dot(fvBinormal, fvLightDirection1.xyz); 
			LightDirection1.z  = dot(fvNormal, fvLightDirection1.xyz);

			float3 fvLightDirection2  = normalize(LightDirection1);
			float LightDistMultiplier = 1.0 / (getLengthSQR(fvLightDirection1) / (fLightStrength[i] * 10000.0)); 
			/// End added pixel

			float fNDotL1            = max(dot(fvNormal2, fvLightDirection2),0.0)-0.1;  
			float3 fvReflection1     = normalize(((2.0 * fvNormal2)) - fvLightDirection2);  
			float3 fvViewDirection2  = normalize(ViewDirection);
			float fRDotV1            = max(0.0, dot(fvReflection1, fvViewDirection2)); 

			fvTotalDiffuse			 += fvLightColor[i] * fNDotL1 * fvBaseColor*LightDistMultiplier;  
			fvTotalSpecular			 += fNDotL1 * fvLightColor[i] * (pow(fRDotV1, fSpecularPower)) * LightDistMultiplier;
		}

#ifdef DIRECT3D_11
		fvTotalSpecular *= fvTotalSpecular + specularMap.Sample(specularMapST, IN.Texcoord.xy) * shininess;
#else
		fvTotalSpecular *= fvTotalSpecular + tex2D(specularMap, IN.Texcoord) * shininess;
#endif
		color = (fvTotalAmbient + fvTotalDiffuse + (fvTotalSpecular*fSpecularStrength));

		if(color.r > 1.0) { color.gb += color.r - 1.0; } 
		if(color.g > 1.0) { color.rb += color.g - 1.0; } 
		if(color.b > 1.0) { color.rg += color.b - 1.0; } 
   
		return color;
	}
}

#endif
