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

uniform int                                 hasDirLightShadowMap;
uniform sampler2D                           dirLightShadowMap;
uniform mat4                                dirLightProjection;
uniform mat4                                dirLightTransformation;

void main() {

#ifdef DEBUG

    if(renderMode > uint(0)) {
        if(renderMode == uint(1)) {
            FragColor     = texture(gPosition, texCoords);
            return;
        } else if (renderMode == uint(2)) {
            FragColor     = texture(gAlbedo, texCoords);
            return;
        } else if (renderMode == uint(3)) {
            FragColor     = texture(gNormal, texCoords);
            return;
        } else {
            float spec     = texture(gAlbedo, texCoords).w;
            FragColor     = vec4(spec, spec, spec, 1.0f);
            return;
        }
    }

#endif


	vec4 color 				= texture(gAlbedo, texCoords);
	vec3 normal_N 			= normalize(texture(gNormal, texCoords).xyz);
	vec3 fragmentPosition 	= texture(gPosition, texCoords).xyz;
	vec3 diffuseColor		= color.xyz;
	vec3 specularColor		= vec3(color.w, color.w, color.w);

	vec3 surfaceCameraZ_N 	= normalize(cameraPosition - fragmentPosition);
    vec3 result = vec3(0.0f);


    vec3 surfaceZCamera_N     = normalize(cameraPosition - fragmentPosition);
    float shininess         = 32.0f;

    result += diffuseColor * 0.01f;

    if (hasDirLight == 1) {
        float shadow = 0.0f;
        if (hasDirLightShadowMap == 1) {
            vec4 fragmentInLightPos = dirLightProjection * dirLightTransformation * vec4(fragmentPosition, 1.0f);
            shadow = calculate_pcf_shadow(dirLightShadowMap, fragmentInLightPos, dirLight.direction, normal_N, 0.005f, 0.05f);
        }
        result += (1.0f - shadow) * count_light_influence(dirLight, diffuseColor, normal_N, mat4(1.0f));
    }


    for(int j = 0; j < spotLightsAmount; ++j) {
        vec4 fragmentInLightPos = spotLightsProjections[j] * spotLightsToLightTransformations[j] * vec4(fragmentPosition, 1.0f);
        float shadow = calculate_pcf_shadow(spotLightsShadowMaps[j], fragmentInLightPos, spotLights[j].direction, normal_N, 0.000005f, 0.0000005f);

        mat3 rot = mat3(spotLightsToLightTransformations[j]);
        vec3 pos = vec3(spotLightsToLightTransformations[j][3][0], spotLightsToLightTransformations[j][3][1], spotLightsToLightTransformations[j][3][2]);
        vec3 transformatedLightPosition = pos * rot;

        vec3 lightDirection             = spotLights[j].direction;
        vec3 surfaceLightZ              = transformatedLightPosition + fragmentPosition;
	    vec3 surfaceLightZ_N            = normalize(surfaceLightZ);
        float distance = length(surfaceLightZ);

        float attenuation = count_attenuation_factor(distance);
        float aa = radians(spotLights[j].angleDegrees);
        float bb = 0.6;

        float epsilon   = aa - bb;
        float intensity = clamp((dot(surfaceLightZ_N, -lightDirection) - bb) / epsilon, 0.0f, 1.0f);
	    result += spotLights[j].color * diffuseColor/*ambient*/ * attenuation * AMBIENT_STRENGTH;
		result += spotLights[j].color * specularColor * count_blinn_phong_specular_factor(surfaceCameraZ_N, surfaceLightZ_N, -normal_N, shininess) * attenuation * SPECULAR_STRENGTH;

        result += (1.0f - shadow) * intensity * spotLights[j].color * diffuseColor * count_diffuse_factor(-normal_N, surfaceLightZ_N) * attenuation * 0.3;

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
