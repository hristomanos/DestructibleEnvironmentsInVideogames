//--------------------------------------------------------------------------------------
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

// the lighting equations in this code have been taken from https://www.3dgep.com/texturing-lighting-directx-11/
// with some modifications by David White

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 vLightDir[2];
	float4 vLightColor[2];
	float4 vOutputColor;
}

Texture2D txDiffuse[3] : register(t0);
SamplerState samLinear : register(s0);

#define MAX_LIGHTS 1
// Light types.
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct _Material
{
	float4  Emissive;       // 16 bytes
							//----------------------------------- (16 byte boundary)
	float4  Ambient;        // 16 bytes
							//------------------------------------(16 byte boundary)
	float4  Diffuse;        // 16 bytes
							//----------------------------------- (16 byte boundary)
	float4  Specular;       // 16 bytes
							//----------------------------------- (16 byte boundary)
	float   SpecularPower;  // 4 bytes
	bool    UseTexture;     // 4 bytes
	bool	UseBumpMap;
	float   Padding;        // 8 bytes
							//----------------------------------- (16 byte boundary)
};  // Total:               // 80 bytes ( 5 * 16 )

cbuffer MaterialProperties : register(b1)
{
	_Material Material;
};

struct Light
{
	float4      Position;               // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4      Direction;              // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4      Color;                  // 16 bytes
										//----------------------------------- (16 byte boundary)
	float       SpotAngle;              // 4 bytes
	float       ConstantAttenuation;    // 4 bytes
	float       LinearAttenuation;      // 4 bytes
	float       QuadraticAttenuation;   // 4 bytes
										//----------------------------------- (16 byte boundary)
	int         LightType;              // 4 bytes
	bool        Enabled;                // 4 bytes
	int2        Padding;                // 8 bytes
										//----------------------------------- (16 byte boundary)
};  // Total:                           // 80 bytes (5 * 16)

cbuffer LightProperties : register(b2)
{
	float4 EyePosition;                 // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4 GlobalAmbient;               // 16 bytes
										//----------------------------------- (16 byte boundary)
	Light Lights[MAX_LIGHTS];           // 80 * 8 = 640 bytes
}; 

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float4 worldPos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 eyeVectorTS : EYETS;
	float3 lightVectorTS: LIGHTTS;
};


float4 DoDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L));
	return light.Color * NdotL;
}

float4 DoSpecular(Light lightObject, float3 vertexToEye, float3 lightDirectionToVertex, float3 Normal)
{
	float4 lightDir = float4(normalize(-lightDirectionToVertex),1);
	vertexToEye = normalize(vertexToEye);

	float lightIntensity = saturate(dot(Normal, lightDir));
	float4 specular = float4(0, 0, 0, 0);
	if (lightIntensity > 0.0f)
	{
		float3  reflection = normalize(2 * lightIntensity * Normal - lightDir);
		specular = pow(saturate(dot(reflection, vertexToEye)), Material.SpecularPower); // 32 = specular power
	}

	return specular;
}

float DoAttenuation(Light light, float d)
{
	return 1.0f / (light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d);
}

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
    float4 Direction;
};

LightingResult DoPointLight(Light light, float3 vertexToEye, float4 vertexPos, float3 N)
{
	LightingResult result;

	float3 LightDirectionToVertex = (vertexPos - light.Position).xyz;
	float distance = length(LightDirectionToVertex);
	LightDirectionToVertex = LightDirectionToVertex  / distance;

	float3 vertexToLight = (light.Position - vertexPos).xyz;
	distance = length(vertexToLight);
	vertexToLight = vertexToLight / distance;

	float attenuation = DoAttenuation(light, distance);
	attenuation = 1;


	result.Diffuse = DoDiffuse(light, vertexToLight, N) * attenuation;
	result.Specular = DoSpecular(light, vertexToEye, LightDirectionToVertex, N) * attenuation;
    result.Direction = float4(normalize(-LightDirectionToVertex), 1);

	return result;
}

LightingResult ComputeLighting(float4 vertexPos, float3 N)
{
	float3 vertexToEye = normalize(EyePosition - vertexPos).xyz;

    LightingResult totalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

	[unroll]
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
        LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

		if (!Lights[i].Enabled) 
			continue;
		
		result = DoPointLight(Lights[i], vertexToEye, vertexPos, N);
		
		totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
        totalResult.Direction += result.Direction;
    }

	totalResult.Diffuse = saturate(totalResult.Diffuse);
    totalResult.Specular = saturate(totalResult.Specular);
    totalResult.Direction = saturate(totalResult.Direction);

	return totalResult;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0; 
    output.Pos = mul( input.Pos, World ); //Model to world
	output.worldPos = output.Pos; //Assign world pos
    output.Pos = mul( output.Pos, View ); //World to view
    output.Pos = mul( output.Pos, Projection ); //View to projection
	
	output.Tex = input.Tex;

	// multiply the normal by the world transform (to go from model space to world space)
   
	float3 vertexToEye = EyePosition - output.worldPos.xyz;
	float3 vertexToLight = Lights[0].Position - output.worldPos.xyz;

	//Build TBN matrix
	output.tangent = mul(input.tangent, World);
	output.Norm = mul(float4(input.Norm, 1.0f), World).xyz;
	output.binormal = normalize(mul(float4(input.binormal, 1.0f),World).xyz);

	float3x3 TBN = float3x3(output.tangent, output.binormal, output.Norm);
	float3x3 TBN_inv = transpose(TBN);

	output.eyeVectorTS = normalize(mul(vertexToEye.xyz, TBN_inv));
	output.lightVectorTS = normalize(mul(vertexToLight.xyz, TBN_inv));

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS(PS_INPUT IN) : SV_TARGET
{
	LightingResult lit;// = ComputeLighting(IN.worldPos, normalize(IN.Norm));


	IN.Norm = normalize(IN.Norm);

	float4 texColor = { 1, 1, 1, 1 };

	if (Material.UseTexture)
	{
		texColor = txDiffuse[0].Sample(samLinear, IN.Tex);
	}

	float4 lightIntensity = { 1, 1, 1, 1 };

	//if (Material.UseBumpMap)
	//{
		//Load normal map
		float4 bumpMap = txDiffuse[1].Sample(samLinear, IN.Tex);

		//Change normal map range from [0,1] to [-1,1]
		bumpMap = (bumpMap * 2.0f) - 1.0f;

		float3 N = IN.Norm;
		float3 T = normalize(IN.tangent - dot(IN.tangent, N) * N);
		float3 B = cross(N, T);
		float3x3 TBN = float3x3(T, B, N);

		float3 bumpedNormal = mul(bumpMap, TBN);
		
		////Make sure tangent is completely orthogonal to normal
		//IN.tangent = normalize(IN.tangent - dot(IN.tangent, IN.Norm) * IN.Norm);

		////Create biTangent
		//float3 biTangent = cross(IN.Norm, IN.tangent);

		////Create the "Texture space"
		//float3x3 texSpace = float3x3(IN.tangent,biTangent,IN.Norm);

		//float3 inNorm = normalize(mul(bumpMap,texSpace));

		//bumpMap = float4(normalize(bumpMap.xyz), 1);

		//lightIntensity = saturate(dot(bumpMap, lit.Direction));

		lit = ComputeLighting(IN.worldPos, normalize(bumpedNormal));
	//}
	//else
	//{
		//lit = ComputeLighting(IN.worldPos, normalize(IN.Norm));

	//}

	


	float4 diffuse = Material.Diffuse * lit.Diffuse;
	float4 ambient = Material.Ambient * GlobalAmbient;

	
	////finalColor = diffuse * ambient;
	//finalColor = lit.Diffuse * GlobalAmbient;
	//finalColor += saturate(dot(lit.Direction, IN.Norm) * diffuse* lit.Diffuse);


	float4 emissive = Material.Emissive;
	float4 specular = Material.Specular * lit.Specular;

	//float4 fHeightMapScale = 0.1f;
	//float fParallaxLimit = -length(IN.eyeVectorTS.xy) / IN.eyeVectorTS.z;
	//	  fParallaxLimit *= fHeightMapScale;
	//
	//float2 vOffsetDir = normalize(IN.eyeVectorTS.xy);
	//float2 vMaxOffset = vOffsetDir * fParallaxLimit;

	//int nNumSamples = (int)lerp(1, 0, dot(IN.Pos - IN.eyeVectorTS, IN.Norm));
	//float fStepSize = 1.0 / (float)nNumSamples;

	//float2 dx = ddx(IN.Tex);
	//float2 dy = ddy(IN.Tex);
	//float fCurrRayHeight = 1.0;

	//float2 vCurrOffset = float2(0, 0);
	//float2 vLastOffset = float2(0, 0);

	//float fLastSampledHeight = 1;
	//float fCurrSampledHeight = 1;

	//int nCurrSample = 0;

	//while (nCurrSample < nNumSamples)
	//{
	//	fCurrSampledHeight = txDiffuse[2].SampleGrad(samLinear, IN.Tex + vCurrOffset, dx, dy).r;
	//	if (fCurrSampledHeight > fCurrRayHeight)
	//	{
	//		float delta1 = fCurrSampledHeight - fCurrRayHeight;
	//		float delta2 = (fCurrRayHeight + fStepSize) - fLastSampledHeight;

	//		float ratio = delta1 / (delta1 + delta2);

	//		vCurrOffset = (ratio)* vLastOffset + (1.0 - ratio) * vCurrOffset;

	//		nCurrSample = nNumSamples + 1;
	//	}
	//	else
	//	{
	//		nCurrSample++;

	//		fCurrRayHeight -= fStepSize;

	//		vLastOffset = vCurrOffset;
	//		vCurrOffset += fStepSize * vMaxOffset;

	//		fLastSampledHeight = fCurrSampledHeight;
	//	}
	//}

	//float2 vFinalCoords = IN.Tex; // +vCurrOffset;

	//float4 vFinalNormal = txDiffuse[1].Sample(samLinear, vFinalCoords);

	//float4 vFinalColor = txDiffuse[0].Sample(samLinear, vFinalCoords);
	//// Expand the final normal vector from [0,1] to [-1,1] range.
	//vFinalNormal = vFinalNormal * 2.0f - 1.0f;

	//float3 vAmbient = vFinalColor.rgb * 0.1f;
	//float3 vDiffuse = vFinalColor.rgb * max(0.0f, dot(lit.Diffuse, vFinalNormal.xyz)) * 0.5f;

	//vFinalColor.rgb = vAmbient + vDiffuse;

	//OUT.color = vFinalColor;*/



	

	

   float4 finalColor = (emissive + ambient + diffuse + specular)* texColor;

	//float4 finalColor;

	//return finalColor;
	return finalColor;

	//return float4(finalColor, diffuse.a);
}

//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid(PS_INPUT input) : SV_Target
{
	return vOutputColor;
}

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}