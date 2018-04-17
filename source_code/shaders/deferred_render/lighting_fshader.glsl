R"(

in 		vec2								texCoords;
in 		vec3 								cameraPosition;

uniform sampler2D 							gPosition;
uniform sampler2D 							gNormal;
uniform sampler2D 							gAlbedo;

uniform	int									spotLightsAmount;
uniform sampler2D [MAX_SPOT_LIGHT_AMOUNT]   spotLightsShadowMaps;
uniform mat4 [MAX_SPOT_LIGHT_AMOUNT]        spotLightsProjections;
uniform mat4 [MAX_SPOT_LIGHT_AMOUNT]        spotLightsToLightTransformations;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;

uniform	int									pointLightsAmount;
uniform	PointLight [MAX_POINT_LIGHT_AMOUNT]	pointLights;

uniform	int 								hasDirLight;
uniform DirectionalLight 					dirLight;

uniform uint								renderMode;

out 	vec4 								FragColor;

uniform sampler2D                           shadowMap;
// TODO refactor name
uniform mat4                                sm_projection;
uniform mat4                                sm_cameraTransformation;

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
	
    float shadow = calculate_pcf_shadow(shadowMap, fragmentInLightPos, dirLight.direction, normal_N, 0.005f, 0.05f);
    
    if (hasDirLight == 1) {
        result += (1.0f - shadow) * count_light_influence(dirLight, diffuseColor, normal_N, mat4(1.0f));
    }

    result += diffuseColor * 0.1f;

    for(int j=0; j < spotLightsAmount; ++j) {
        vec4 fm = spotLightsProjections[j] * spotLightsToLightTransformations[j] * vec4(fragmentPosition, 1.0f);

        float sh = calculate_pcf_shadow(spotLightsShadowMaps[0], fm, spotLights[j].direction, normal_N, 0.00000001f, 0.000001f);

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

    FragColor = vec4(result, 1.0f);

}

)";
