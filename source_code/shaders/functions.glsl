R"(

#define A_PARAM 					0.1f
#define B_PARAM 					0.01f
#define C_PARAM						1.0f
#define MIN_ATTENUATION				0.01f

#define AMBIENT_STRENGTH			0.05f
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
	float 		power;
	vec3 		color;
	vec3		position;
	float		angleDegrees;
	vec3		direction;
};


					//////////////////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////////////
					//																					//
					//	I had to find a way how to indicate in which side of direction vector points,	//
					//	so I decided to do this in such way												//
					//	We are given vec A and B														//
					//	I mark subtraction A - B as "ZAB" which means that it is a vector,				//
					//	which was created as a product of subtraction two vectors and points to A		//
					//																					//
					//	I mark normalized vectors with "_N" at the end of name							//
					//////////////////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////////////

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

vec3 count_light_influence(vec3         lightColor,
						   vec3 		ambientColor,
						   vec3 		diffuseColor,
						   vec3 		specularColor,
						   float		shininess,
						   vec3 		normal_N,
						   vec3 		fragmentZCamera_N,
						   vec3			surfaceZLight,
						   vec3			surfaceZLight_N) {
	
	
	//TODO USE LENGTH2 INSTEAD OF LENGTH
	float distance = length(surfaceZLight);
		
	float attenuation = count_attenuation_factor(distance);
	
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	
	result += lightColor * ambientColor * attenuation * AMBIENT_STRENGTH;
	
	if(attenuation > MIN_ATTENUATION) {
		result += lightColor * diffuseColor * count_diffuse_factor(normal_N, surfaceZLight_N) * attenuation * DIFFUSE_STRENGTH;
		result += lightColor * specularColor * count_blinn_phong_specular_factor(fragmentZCamera_N, surfaceZLight_N, normal_N, shininess) * attenuation * SPECULAR_STRENGTH;
	}
	
	return result;
	
}

vec3 count_light_influence(PointLight 	light,
						   vec3 		surfacePosition,
						   vec3 		ambientColor,
						   vec3 		diffuseColor,
						   vec3 		specularColor,
						   float		shininess,
						   vec3 		normal_N,
						   vec3 		cameraPosition,
						   vec3 		fragmentZCamera_N,
                           mat4         lightTransformation) {
	
    vec3 transformatedLightPosition = (lightTransformation * vec4(light.position, 1.0f)).xyz;
	vec3 surfaceZLight = transformatedLightPosition - surfacePosition;
	vec3 surfaceZLight_N = normalize(surfaceZLight);
	return count_light_influence(light.color,
								 ambientColor,
								 diffuseColor,
								 specularColor,
								 shininess,
								 normal_N,
								 fragmentZCamera_N,
								 surfaceZLight,
								 surfaceZLight_N);
	
}

vec3 count_light_influence(DirectionalLight light, vec3 diffuseColor, vec3 normal_N, mat4 lightTransformation) {
    vec3 lightDirection = normalize(mat3(lightTransformation) * light.direction);
	return light.color.xyz * diffuseColor * count_diffuse_factor(normal_N, lightDirection) * DIFFUSE_STRENGTH;
}

vec3 count_light_influence(SpotLight    light,
                           vec3         surfacePosition,
                           vec3         ambientColor,
                           vec3         diffuseColor,
                           vec3         specularColor,
                           float        shininess,
                           vec3         normal_N,
                           vec3         cameraPosition,
                           vec3         fragmentZCamera_N,
                           mat4         lightTransformation) {

    vec3 transformatedLightPosition = (lightTransformation * vec4(light.position, 1.0f)).xyz;
    vec3 lightDirection             = normalize(mat3(lightTransformation) * light.direction);
    vec3 lightZFragment             = surfacePosition - transformatedLightPosition;
    vec3 lightZFragment_N           = normalize(lightZFragment);

    float angleRadians = radians(max(dot(lightZFragment, lightDirection), 0.0f));

    vec3 color = count_light_influence(light.color,
                                       ambientColor,
                                       diffuseColor,
                                       specularColor,
                                       shininess,
                                       normal_N,
                                       fragmentZCamera_N,
                                       -lightZFragment,
                                       -lightZFragment_N);

    float epsilon   = SPOT_LIGHT_OUTER_CUTOFF - SPOT_LIGHT_INNER_CUTOFF;
    float intensity = clamp((dot(lightZFragment_N, lightDirection) - SPOT_LIGHT_INNER_CUTOFF) / epsilon, 0.0f, 1.0f);

    return intensity * color;

}

vec3 color_with_exposure(vec3 color, float exposure) {
	return 1.0f - exp2(-color * exposure);
}

float get_luminance(vec3 color) {
	return dot(color, vec3(RED_VALUE_WEIGHT, GREEN_VALUE_WEIGHT, BLUE_VALUE_WEIGHT));
}

float get_luminance_for_adaptive_tone(vec3 color) {
	return dot(color, vec3(RED_WEIGHT_ADAPTIVE_TONE, GREEN_WEIGHT_ADAPTIVE_TONE, BLUE_WEIGHT_ADAPTIVE_TONE));
}

float length2(vec2 v) {
	return dot(v, v);
}

float length2(vec3 v) {
	return dot(v, v);
}

float length2(vec4 v) {
	return dot(v, v);
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

    return pcf_depth(textureToSample, projectedCoordinates.xy, 5, 5, currentDepth, bias);
}

float calculate_no_filter_shadow(sampler2D    textureToSample,
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
    
    float closestDepth = texture(textureToSample, projectedCoordinates.xy).r;
    float currentDepth = projectedCoordinates.z;
    
    float bias = max(maxBias * (1.0f - dot(surfaceNormal_N, lightDirection_N)), minBias);
    
    return currentDepth - bias > closestDepth ? 1.0 : 0.0f;
}


)";
