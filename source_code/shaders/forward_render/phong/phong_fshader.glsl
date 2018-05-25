R"(

uniform	int									spotLightsAmount;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;
uniform sampler2D [MAX_SPOT_LIGHT_AMOUNT]   spotLightsShadowMaps;

uniform	int									pointLightsAmount;
uniform	PointLight [MAX_POINT_LIGHT_AMOUNT]	pointLights;

uniform	int 								hasDirLight;
uniform DirectionalLight 					dirLight;

uniform int 								hasMaterial;

layout (std140) uniform MaterialBlock {
	vec3 	ambient;
	vec3 	diffuse;
	vec3 	specular;
	float 	shininess;
	float	opacity;
} materialBlock;

uniform int 								hasDiffuseTexture;
uniform sampler2D 							diffuseTexture;

uniform int 								hasSpecularTexture;
uniform sampler2D 							specularTexture;

uniform int                                 hasNormalTexture;
uniform sampler2D                           normalTexture;

uniform int                                 hasDirLightShadowMap;
uniform sampler2D                           dirLightShadowMap;
uniform mat4                                dirLightProjection;
uniform mat4                                dirLightTransformation;

out vec4 FragColor;

in vec2 texCoord;

in vec3 normal_N;

in vec3 fragmentPosition;
in vec3 fragmentPositionWorld;
in vec3 cameraPosition;
in vec3 surfaceZCamera_N;
in mat4 TBNMat;
in mat4 lightTransformationMatrix;

void main(){
	
	vec3 	ambientColor;
	vec3 	diffuseColor;
	vec3 	specularColor;
    vec3    normalToUse_N;
	float 	shininess;
	vec3 	result = vec3(0.0f);

	
	if(hasMaterial == 1) {
		diffuseColor 	= hasDiffuseTexture == 1 ? texture(diffuseTexture, texCoord).xyz : materialBlock.diffuse;
        ambientColor    = diffuseColor;
		specularColor 	= hasSpecularTexture == 1 ? texture(specularTexture, texCoord).xyz : materialBlock.specular;
		shininess		= materialBlock.shininess;
	} else {
		// some default colors
        ambientColor    = vec3(0.0f, 0.0f, 0.0f);
        diffuseColor 	= vec3(0.0f, 0.0f, 0.0f);
		specularColor 	= vec3(0.0f, 0.0f, 0.0f);
		shininess		= 1.0f;
	}
    
    if(hasNormalTexture == 1) {
        normalToUse_N = texture(normalTexture, texCoord).xyz;
        normalToUse_N = normalize(normalToUse_N * 2.0 - 1.0);
    } else {
        normalToUse_N = normal_N;
    }

    result += diffuseColor * 0.1f;
    vec3 normalInWorld = (inverse(TBNMat) * vec4(normalToUse_N, 1.0f)).xyz;

	if (hasDirLight == 1) {
        float shadow = 0.0f;
        if (hasDirLightShadowMap == 1) {
            vec4 fragmentInLightPos = dirLightProjection * dirLightTransformation * vec4(fragmentPositionWorld, 1.0f);
            shadow = calculate_pcf_shadow(dirLightShadowMap, fragmentInLightPos, dirLight.direction, normalInWorld, 0.005f, 0.05f);
        }
        result += dirLight.power/USUAL_POWER * (1.0f - shadow) * count_light_influence(dirLight, diffuseColor, normalToUse_N, lightTransformationMatrix);
	}
	
    for(int j=0; j < spotLightsAmount; ++j) {
        vec3 lightDirection_N = normalize(vec3(spotLights[j].transformation[0][2], spotLights[j].transformation[1][2], spotLights[j].transformation[2][2]));
        mat3 spotLightRotation = mat3(spotLights[j].transformation);
        vec3 spotLightPosition = vec3(spotLights[j].transformation[3][0], spotLights[j].transformation[3][1], spotLights[j].transformation[3][2]);
        vec3 transformatedLightPosition = spotLightPosition * spotLightRotation;

        
        vec3 surfaceLightZ = -transformatedLightPosition - fragmentPositionWorld;
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
            vec4 fragmentInLightPos = spotLights[j].projection * spotLights[j].transformation * vec4(fragmentPositionWorld, 1.0f);
            
            float shadow = spotLights[j].castsShadow == 0 ? 0.0f : calculate_pcf_shadow(spotLightsShadowMaps[0], fragmentInLightPos, -lightDirection_N, normal_N, 0.000001f);
            
            res += spotLights[j].color * diffuseColor * count_diffuse_factor(normal_N, surfaceLightZ_N) * attenuation * DIFFUSE_STRENGTH;
            res += spotLights[j].color * specularColor * count_blinn_phong_specular_factor(surfaceZCamera_N, surfaceLightZ_N, normal_N, shininess) * attenuation * SPECULAR_STRENGTH;
            
            res = (1.0f - shadow) * intensity * res;
        }

        result += res;
    }
    
    for (int i = 0; i < pointLightsAmount; ++i) {
        result += count_light_influence(pointLights[i],
                                        fragmentPosition,
                                        ambientColor,
                                        diffuseColor,
                                        specularColor,
                                        shininess,
                                        normalToUse_N,
                                        cameraPosition,
                                        surfaceZCamera_N,
                                        lightTransformationMatrix) * pointLights[i].power/USUAL_POWER;
    }

	FragColor = vec4(result, materialBlock.opacity);
	
}

)";
