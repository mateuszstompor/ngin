R"(

#define A_PARAM 					0.1f
#define B_PARAM 					0.01f
#define C_PARAM						1.0f
#define MIN_ATTENUATION				0.01f

#define AMBIENT_STRENGTH			0.04f
#define DIFFUSE_STRENGTH			0.9f
#define SPECULAR_STRENGTH			0.8f

#define SPOT_LIGHT_INNER_CUTOFF		0.91f
#define SPOT_LIGHT_OUTER_CUTOFF		0.50f

#define RED_VALUE_WEIGHT			0.2126
#define GREEN_VALUE_WEIGHT			0.7152
#define BLUE_VALUE_WEIGHT			0.0722

#define RED_WEIGHT_ADAPTIVE_TONE	0.3
#define GREEN_WEIGHT_ADAPTIVE_TONE	0.59
#define BLUE_WEIGHT_ADAPTIVE_TONE	0.11


struct DirectionalLight {
	vec3    	direction;
	vec3    	color;
};


//position in world cooridnates
struct PointLight {
	float 		power;
	vec3		color;
	vec3		position;
};

//position in world cooridnates
struct SpotLight {
    mat4        transformation;
    mat4        projection;
	vec3 		color;
	float 		power;
	float		angleDegrees;
	int         castsShadow;
};

float count_diffuse_factor(vec3 normal_N, vec3 surfaceZLight_N) {
	return max(dot(normal_N, surfaceZLight_N), 0.0f);
}

float count_phong_specular_factor(vec3 surfaceZCamera_N, vec3 surfaceZLight_N, vec3 normal_N, float shininess) {
	return pow(max(0.0f, dot(surfaceZCamera_N, reflect(-surfaceZLight_N, normal_N))), shininess);
}

float count_blinn_phong_specular_factor(vec3 surfaceZCamera_N, vec3 surfaceZLight_N, vec3 normal_N, float shininess) {
	vec3 H = normalize(surfaceZCamera_N + surfaceZLight_N);
	return pow(max(0.0f, dot(H, normal_N)), shininess);
}

float count_attenuation_factor(float distance) {
	return 1.0f/(B_PARAM * distance * distance + A_PARAM * distance + C_PARAM);
}

//vec3 count_light_influence(vec3         lightColor,
//						   vec3 		ambientColor,
//						   vec3 		diffuseColor,
//						   vec3 		specularColor,
//						   float		shininess,
//						   vec3 		normal_N,
//						   vec3 		fragmentZCamera_N,
//						   vec3			surfaceZLight,
//						   vec3			surfaceZLight_N) {
//
//
//	//TODO USE LENGTH2 INSTEAD OF LENGTH
//	float distance = length(surfaceZLight);
//
//	float attenuation = count_attenuation_factor(distance);
//
//	vec3 result = vec3(0.0f, 0.0f, 0.0f);
//
//	result += lightColor * ambientColor * attenuation * AMBIENT_STRENGTH;
//
//	if(attenuation > MIN_ATTENUATION) {
//		result += lightColor * diffuseColor * count_diffuse_factor(normal_N, surfaceZLight_N) * attenuation * DIFFUSE_STRENGTH;
//		result += lightColor * specularColor * count_blinn_phong_specular_factor(fragmentZCamera_N, surfaceZLight_N, normal_N, shininess) * attenuation * SPECULAR_STRENGTH;
//	}
//
//	return result;
//
//}

//vec3 count_light_influence(PointLight 	light,
//						   vec3 		surfacePosition,
//						   vec3 		ambientColor,
//						   vec3 		diffuseColor,
//						   vec3 		specularColor,
//						   float		shininess,
//						   vec3 		normal_N,
//						   vec3 		cameraPosition,
//						   vec3 		fragmentZCamera_N,
//                           mat4         lightTransformation) {
//
//    vec3 transformatedLightPosition = (lightTransformation * vec4(light.position, 1.0f)).xyz;
//	vec3 surfaceZLight = transformatedLightPosition - surfacePosition;
//	vec3 surfaceZLight_N = normalize(surfaceZLight);
//	return count_light_influence(light.color,
//								 ambientColor,
//								 diffuseColor,
//								 specularColor,
//								 shininess,
//								 normal_N,
//								 fragmentZCamera_N,
//								 surfaceZLight,
//								 surfaceZLight_N);
//
//}

vec3 count_light_influence(DirectionalLight light, vec3 diffuseColor, vec3 normal_N, mat4 lightTransformation) {
    vec3 lightDirection = normalize(mat3(lightTransformation) * light.direction);
	return light.color.xyz * diffuseColor * count_diffuse_factor(normal_N, lightDirection) * DIFFUSE_STRENGTH;
}

float pcf_depth(sampler2D textureToSample,
                vec2 sampleCoordinate,
                int rowSamples,
                int columnSamples,
                float countedDepth,
                float bias) {

    vec2 texelSize = vec2(1.0f) / vec2(textureSize(textureToSample, 0));
    float result = 0.0f;

    for (int i = -rowSamples; i <= rowSamples; ++i) {
        for (int j = -columnSamples; j <= columnSamples; ++j) {
            float depth = texture(textureToSample, sampleCoordinate + vec2(i, j) * texelSize).r;
            result += countedDepth - bias > depth ? 1.0 : 0.0f;
        }
    }

    return result/float((rowSamples * 2 + 1) * (columnSamples * 2 + 1));
}

float calculate_pcf_shadow(sampler2D    textureToSample,
                           vec4         fragmentPositionInLightSpace,
                           vec3         lightDirection_N,
                           vec3         surfaceNormal_N,
                           float        minBias,
                           float        maxBias) {
    // TODO do separate function for directional lights
    // it is required to do this division for non-orthographic projections
    vec3 projectedCoordinates = (fragmentPositionInLightSpace.xyz / fragmentPositionInLightSpace.w) / 2.0f + 0.5f;
    // we need to map value from range [0, 1] to [-1, 1]
    if(projectedCoordinates.z > 1.0f) {
        return 0.0f;
    }
    float currentDepth = projectedCoordinates.z;
    float bias = max(maxBias * (1.0f - dot(surfaceNormal_N, lightDirection_N)), minBias);

    return pcf_depth(textureToSample, projectedCoordinates.xy, 3, 3, currentDepth, bias);
}



#define MAX_SPOT_LIGHT_AMOUNT	12
#define MAX_POINT_LIGHT_AMOUNT	12
in 		vec2								texCoords;
in 		vec3 								cameraPosition;

uniform sampler2D 							gPosition;
uniform sampler2D 							gNormal;
uniform sampler2D 							gAlbedo;

uniform	int									spotLightsAmount;
uniform sampler2D [MAX_SPOT_LIGHT_AMOUNT]   spotLightsShadowMaps;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;

uniform	int									pointLightsAmount;
uniform	PointLight [MAX_POINT_LIGHT_AMOUNT]	pointLights;

uniform	int 								hasDirLight;
uniform DirectionalLight 					dirLight;

out 	vec4 								outputColor;

uniform int                                 hasDirLightShadowMap;
uniform sampler2D                           dirLightShadowMap;
uniform mat4                                dirLightProjection;
uniform mat4                                dirLightTransformation;

void main() {

	vec4 color 				= texture(gAlbedo, texCoords);
	vec3 normal_N 			= normalize(texture(gNormal, texCoords).xyz);
	vec3 fragmentPosition 	= texture(gPosition, texCoords).xyz;
	vec3 diffuseColor		= color.xyz;
	vec3 specularColor		= vec3(color.w, color.w, color.w);

	vec3 surfaceCameraZ_N 	= normalize(cameraPosition - fragmentPosition);
    vec3 result = vec3(0.0f);


    vec3 surfaceZCamera_N     = normalize(cameraPosition - fragmentPosition);
    float shininess         = 32.0f;

    result += diffuseColor * 0.04f;

    if (hasDirLight == 1) {
        float shadow = 0.0f;
        if (hasDirLightShadowMap == 1) {
            vec4 fragmentInLightPos = dirLightProjection * dirLightTransformation * vec4(fragmentPosition, 1.0f);
            shadow = calculate_pcf_shadow(dirLightShadowMap, fragmentInLightPos, dirLight.direction, normal_N, 0.005f, 0.05f);
        }
        result += (1.0f - shadow) * count_light_influence(dirLight, diffuseColor, normal_N, mat4(1.0f));
    }


    for(int j = 0; j < spotLightsAmount; ++j) {
        vec4 fragmentInLightPos = spotLights[j].projection * spotLights[j].transformation * vec4(fragmentPosition, 1.0f);
        vec3 lightDirection = normalize(vec3(spotLights[j].transformation[0][2], spotLights[j].transformation[1][2], spotLights[j].transformation[2][2]));
        mat3 rot = mat3(spotLights[j].transformation);
        vec3 pos = vec3(spotLights[j].transformation[3][0], spotLights[j].transformation[3][1], spotLights[j].transformation[3][2]);
        vec3 transformatedLightPosition = pos * rot;

//shadow

//vec3 projectedCoordinates = (fragmentInLightPos.xyz / fragmentInLightPos.w) / 2.0f + 0.5f;
//    // we need to map value from range [0, 1] to [-1, 1]
//    float currentDepth = projectedCoordinates.z;
//
//    float depth = texture(spotLightsShadowMaps[j], projectedCoordinates.xy).r;
//    float shadow = currentDepth < depth ? 1.0 : 0.0f;

//shadow



        float shadow = spotLights[j].castsShadow == 0 ? 0.0f : calculate_pcf_shadow(spotLightsShadowMaps[j], fragmentInLightPos, -lightDirection, normal_N, 0.00001f, 0.000001f);

        vec3 surfaceLightZ = -transformatedLightPosition - fragmentPosition;
	    vec3 surfaceLightZ_N = normalize(surfaceLightZ);
        float distance = length(surfaceLightZ);
        float attenuation = count_attenuation_factor(distance);

        float aa = radians(spotLights[j].angleDegrees);
        float bb = aa - 0.2f;

        vec3 res = vec3(0.0f);
        float epsilon   = aa - bb;
        float intensity = clamp((dot(surfaceLightZ_N, lightDirection) - bb) / epsilon, 0.0f, 1.0f);

        //ambient
        result += spotLights[j].color * diffuseColor/*ambient*/ * attenuation * AMBIENT_STRENGTH;

        if (intensity > 0.001f) {
            res += spotLights[j].color * diffuseColor * count_diffuse_factor(normal_N, surfaceLightZ_N) * attenuation * DIFFUSE_STRENGTH;
            res += spotLights[j].color * specularColor * count_blinn_phong_specular_factor(surfaceCameraZ_N, surfaceLightZ_N, normal_N, shininess) * attenuation * SPECULAR_STRENGTH;
        }

        result += (1.0f - shadow) * intensity * res;
    }


//    for (int i = 0; i < pointLightsAmount; ++i) {
//        result += count_light_influence(pointLights[i],
//                                        fragmentPosition,
//                                        diffuseColor,
//                                        diffuseColor,
//                                        specularColor,
//                                        shininess,
//                                        normal_N,
//                                        cameraPosition,
//                                        surfaceZCamera_N,
//                                        mat4(1.0f));
//    }
//
    outputColor = vec4(result, 1.0f);

}

)";
