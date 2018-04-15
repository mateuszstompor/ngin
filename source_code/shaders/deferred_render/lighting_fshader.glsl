R"(

in 		vec2								texCoords;
in 		vec3 								cameraPosition;

uniform sampler2D 							gPosition;
uniform sampler2D 							gNormal;
uniform sampler2D 							gAlbedo;
uniform sampler2D                           shadowMap;

uniform sampler2D                           spotLightsShadowMaps[5];

uniform	int									spotLightsAmount;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;

uniform	int									pointLightsAmount;
uniform	PointLight [MAX_POINT_LIGHT_AMOUNT]	pointLights;

uniform	int 								hasDirLight;
uniform DirectionalLight 					dirLight;

uniform mat4                                sm_projection;
uniform mat4                                sm_cameraTransformation;

uniform mat4                                spot_sm_projection[5];
uniform mat4                                spot_sm_cameraTransformation[5];

uniform uint								renderMode;

out 	vec4 								FragColor;

float pcf_depth(sampler2D tex, vec2 position, int rowSamples, int columnSamples, float countedDepth, float bias) {
    vec2 texelSize = vec2(1.0f) / vec2(textureSize(shadowMap, 0));
    float result = 0.0f;
    
    for (int i = -rowSamples; i <= rowSamples; ++i) {
        for (int j = -columnSamples; j <= columnSamples; ++j) {
            float depth = texture(tex, position + vec2(i, j) * texelSize).r;
            result += countedDepth - bias > depth ? 1.0 : 0.0f;
        }
    }
    
    return result/float((rowSamples * 2 + 1) * (columnSamples * 2 + 1));
}

float calculate_shadow(sampler2D tex, vec4 fragPosLightSpace, vec3 lightDir, vec3 normal) {
    // it is required to do this division for non-orthographic projections
    vec3 projectedCoordinates = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // we need to map value from range [0, 1] to [-1, 1]
    projectedCoordinates = projectedCoordinates * 0.5f + 0.5f;
    float closestDepth = texture(tex, projectedCoordinates.xy).r;
    float currentDepth = projectedCoordinates.z;
    float bias = max(0.05f * (1.0f - dot(normal, lightDir)), 0.005f);
    return pcf_depth(shadowMap, projectedCoordinates.xy, 5, 5, currentDepth, bias);
}

void main() {

#ifdef DEBUG

	if(renderMode > uint(0)) {
		if(renderMode == uint(1)) {
			FragColor 	= texture(gPosition, texCoords);
			return;
		} else if (renderMode == uint(2)) {
			FragColor 	= texture(gAlbedo, texCoords);
			return;
		} else if (renderMode == uint(3)) {
			FragColor 	= texture(gNormal, texCoords);
			return;
		} else {
			float spec 	= texture(gAlbedo, texCoords).w;
			FragColor 	= vec4(spec, spec, spec, 1.0f);
			return;
		}
	}

#endif

	vec4 color 				= texture(gAlbedo, texCoords);
	vec3 normal_N 			= texture(gNormal, texCoords).xyz;
	vec3 fragmentPosition 	= texture(gPosition, texCoords).xyz;
	vec3 diffuseColor		= color.xyz;
	vec3 specularColor		= vec3(color.w, color.w, color.w);
	
	vec3 result 			= vec3(0.0f, 0.0f, 0.0f);
	
    vec4 fragmentInLightPos = sm_projection * sm_cameraTransformation * vec4(fragmentPosition, 1.0f);
    
	vec3 surfaceZCamera_N 	= normalize(cameraPosition - fragmentPosition);
	float shininess 		= 32.0f;
	
    float shadow = calculate_shadow(shadowMap, fragmentInLightPos, dirLight.direction, normal_N);
    
    if (hasDirLight == 1) {
        result += diffuseColor * 0.1f + (1.0f - shadow) * count_light_influence(dirLight, diffuseColor, normal_N, mat4(1.0f));
    }
    
    for(int j=0; j < spotLightsAmount; ++j) {
        vec4 fm = spot_sm_projection[j] * spot_sm_cameraTransformation[j] * vec4(fragmentPosition, 1.0f);

        float sh = calculate_shadow(spotLightsShadowMaps[j], fragmentInLightPos, spotLights[j].direction, normal_N);

        
        result += (1.0f - sh) * count_light_influence(spotLights[j],
                                        fragmentPosition,
                                        diffuseColor,
                                        diffuseColor,
                                        specularColor,
                                        shininess,
                                        normal_N,
                                        cameraPosition,
                                        surfaceZCamera_N,
                                        mat4(1.0f));
    }

    for (int i = 0; i < pointLightsAmount; ++i) {
        result += count_light_influence(pointLights[i],
                                        fragmentPosition,
                                        diffuseColor,
                                        diffuseColor,
                                        specularColor,
                                        shininess,
                                        normal_N,
                                        cameraPosition,
                                        surfaceZCamera_N,
                                        mat4(1.0f));
    }

    FragColor = vec4(result, 1.0f);

}

)";
