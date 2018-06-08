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

#define RED_VALUE_WEIGHT			0.2126f
#define GREEN_VALUE_WEIGHT			0.7152f
#define BLUE_VALUE_WEIGHT			0.0722f

#define RED_WEIGHT_ADAPTIVE_TONE	0.3f
#define GREEN_WEIGHT_ADAPTIVE_TONE	0.59f
#define BLUE_WEIGHT_ADAPTIVE_TONE	0.11f

#define INNER_CUTOFF_BIAS           0.015f
#define USUAL_POWER                 100.0f

struct DirectionalLight {
    float       power;
	vec3    	direction;
	vec3    	color;
};

//position in world cooridnates
struct PointLight {
	float 		power;
	vec3		color;
	vec3		position;
    mat4        transformation;
    mat4        projection;
    int         castsShadow;
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

vec3 count_light_influence(vec3     lightColor,
                           vec3     ambientColor,
                           vec3     diffuseColor,
                           vec3     specularColor,
                           float    shininess,
                           vec3     normal_N,
                           vec3     fragmentZCamera_N,
                           vec3     surfaceZLight,
                           vec3     surfaceZLight_N) {


    //TODO USE LENGTH2 INSTEAD OF LENGTH
    float distance = length(surfaceZLight);

    float attenuation = count_attenuation_factor(distance);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);


    if(attenuation > MIN_ATTENUATION) {
        result += lightColor * diffuseColor * count_diffuse_factor(normal_N, surfaceZLight_N) * attenuation * DIFFUSE_STRENGTH;
        result += lightColor * specularColor * count_blinn_phong_specular_factor(fragmentZCamera_N, surfaceZLight_N, normal_N, shininess) * attenuation * SPECULAR_STRENGTH;
    }

    return result;

}

float pcf_depth(in sampler2D    textureToSample,
                vec2            sampleCoordinate,
                int             rowSamples,
                int             columnSamples,
                float           countedDepth,
                float           bias) {

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

float pcf_depth(sampler2DArray  textureToSample,
                int             layer,
                vec2            sampleCoordinate,
                int             rowSamples,
                int             columnSamples,
                float           countedDepth,
                float           bias) {
    
    vec2 texelSize = vec2(1.0f) / vec2(textureSize(textureToSample, 0));
    float result = 0.0f;
    
    for (int i = -rowSamples; i <= rowSamples; ++i) {
        for (int j = -columnSamples; j <= columnSamples; ++j) {
            float depth = texture(textureToSample, vec3(sampleCoordinate + vec2(i, j) * texelSize, layer)).r;
            result += countedDepth - bias > depth ? 1.0 : 0.0f;
        }
    }
    
    return result/float((rowSamples * 2 + 1) * (columnSamples * 2 + 1));
}

float calculate_pcf_shadow(in sampler2D     textureToSample,
                           vec4             fragmentPositionInLightSpace,
                           vec3             lightDirection_N,
                           vec3             surfaceNormal_N,
                           float            minBias,
                           float            maxBias) {
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

float calculate_pcf_shadow(sampler2D    textureToSample,
                           vec4         fragmentPositionInLightSpace,
                           vec3         lightDirection_N,
                           vec3         surfaceNormal_N,
                           float        bias) {
    // TODO do separate function for directional lights
    // it is required to do this division for non-orthographic projections
    vec3 projectedCoordinates = (fragmentPositionInLightSpace.xyz / fragmentPositionInLightSpace.w) / 2.0f + 0.5f;
    // we need to map value from range [0, 1] to [-1, 1]
    if(projectedCoordinates.z > 1.0f) {
        return 0.0f;
    }
    float currentDepth = projectedCoordinates.z;
    return pcf_depth(textureToSample, projectedCoordinates.xy, 3, 3, currentDepth, bias);
}

float calculate_pcf_shadow(in sampler2DArray    textureToSample,
                           int                  layer,
                           vec4                 fragmentPositionInLightSpace,
                           vec3                 lightDirection_N,
                           vec3                 surfaceNormal_N,
                           float                bias) {
    // TODO do separate function for directional lights
    // it is required to do this division for non-orthographic projections
    vec3 projectedCoordinates = (fragmentPositionInLightSpace.xyz / fragmentPositionInLightSpace.w) / 2.0f + 0.5f;
    // we need to map value from range [0, 1] to [-1, 1]
    if(projectedCoordinates.z > 1.0f) {
        return 0.0f;
    }
    float currentDepth = projectedCoordinates.z;
    return pcf_depth(textureToSample, layer, projectedCoordinates.xy, 3, 3, currentDepth, bias);
}

#define MAX_SPOT_LIGHT_AMOUNT	12
#define MAX_POINT_LIGHT_AMOUNT	12

in 		vec2								texCoords;
in 		vec3 								cameraPosition;

uniform sampler2D 							gPosition;
uniform sampler2D 							gNormal;
uniform sampler2D 							gAlbedo;

uniform	int									spotLightsAmount;
uniform sampler2DArray                      spotLightsShadowMaps;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;

uniform samplerCube                         pointLightShadow;
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

    float shininess         = 32.0f;

    result += diffuseColor * 0.06f;

    if (hasDirLight == 1) {
        float shadow = 0.0f;
        if (hasDirLightShadowMap == 1) {
            vec4 fragmentInLightPos = dirLightProjection * dirLightTransformation * vec4(fragmentPosition, 1.0f);
            shadow = calculate_pcf_shadow(dirLightShadowMap, fragmentInLightPos, dirLight.direction, normal_N, 0.005f, 0.05f);
        }
        result += dirLight.power/USUAL_POWER * (1.0f - shadow) * dirLight.color.xyz * diffuseColor * count_diffuse_factor(normal_N, normalize(dirLight.direction)) * DIFFUSE_STRENGTH;
    }


    for(int j = 0; j < spotLightsAmount; ++j) {
        
        vec3 lightDirection_N = normalize(vec3(spotLights[j].transformation[0][2], spotLights[j].transformation[1][2], spotLights[j].transformation[2][2]));
        mat3 spotLightRotation = mat3(spotLights[j].transformation);
        vec3 spotLightPosition = vec3(spotLights[j].transformation[3][0], spotLights[j].transformation[3][1], spotLights[j].transformation[3][2]);
        vec3 transformatedLightPosition = spotLightPosition * spotLightRotation;
        

        vec3 surfaceLightZ = -transformatedLightPosition - fragmentPosition;
        float distance = length(surfaceLightZ);
        vec3 surfaceLightZ_N = surfaceLightZ / distance;
        float attenuation = count_attenuation_factor(distance) * spotLights[j].power/USUAL_POWER;

        float spotLightAngleRadians = radians(spotLights[j].angleDegrees) + 0.05f;

        vec3 res = vec3(0.0f);

        //ambient
        result += spotLights[j].color * diffuseColor * attenuation * AMBIENT_STRENGTH;
        float dot_product = dot(surfaceLightZ_N, lightDirection_N);
        if(dot_product > spotLightAngleRadians) {
            float intensity = 1.0f;
            if (dot_product - INNER_CUTOFF_BIAS < spotLightAngleRadians) {
                intensity = clamp((dot_product - spotLightAngleRadians)/INNER_CUTOFF_BIAS, 0.0f, 1.0f);
            }
            
            vec4 fragmentInLightPos = spotLights[j].projection * spotLights[j].transformation * vec4(fragmentPosition, 1.0f);
            
            float shadow = spotLights[j].castsShadow == 0 ? 0.0f : calculate_pcf_shadow(spotLightsShadowMaps, j, fragmentInLightPos, -lightDirection_N, normal_N, 0.000001f);

            res += spotLights[j].color * diffuseColor * count_diffuse_factor(normal_N, surfaceLightZ_N) * attenuation * DIFFUSE_STRENGTH;
            res += spotLights[j].color * specularColor * count_blinn_phong_specular_factor(surfaceCameraZ_N, surfaceLightZ_N, normal_N, shininess) * attenuation * SPECULAR_STRENGTH;
            res = (1.0f - shadow) * intensity * res;
        }

        result += res;
    }

    for (int i = 0; i < pointLightsAmount; ++i) {

        mat3 pointLightRotation = mat3(pointLights[i].transformation);
        vec3 pointLightPosition = vec3(pointLights[i].transformation[3][0], pointLights[i].transformation[3][1], pointLights[i].transformation[3][2]);
        vec3 transformatedLightPosition = pointLightPosition * pointLightRotation;

        vec3 surfaceLightZ = - transformatedLightPosition - fragmentPosition;
        vec3 surfaceLightZ_N = normalize(surfaceLightZ);

        float shadow = 0.0f;
        if (pointLights[i].castsShadow == 1) {
            vec4 fragmentInLightPos = pointLights[i].transformation * vec4(fragmentPosition, 1.0f);
            float currentDepth = length(fragmentInLightPos.xyz)/100.0f;
            vec3 ls = vec3(-surfaceLightZ_N.x, surfaceLightZ_N.y, surfaceLightZ_N.z);
            float depthOfTexture = texture(pointLightShadow, ls).r;
            shadow = currentDepth - 0.01f > depthOfTexture ? 1.0 : 0.0f;
        }
        result += pointLights[i].color * diffuseColor * AMBIENT_STRENGTH;
        result += (1.0f - shadow) * count_light_influence(pointLights[i].color,
                                                          diffuseColor,
                                                          diffuseColor,
                                                          specularColor,
                                                          shininess,
                                                          normal_N,
                                                          surfaceCameraZ_N,
                                                          surfaceLightZ,
                                                          surfaceLightZ_N) * pointLights[i].power/USUAL_POWER;
    }

    outputColor = vec4(result, 1.0f);

}

)";
