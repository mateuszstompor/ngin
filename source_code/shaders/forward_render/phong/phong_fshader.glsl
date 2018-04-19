R"(

uniform	int									spotLightsAmount;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;
uniform sampler2D [MAX_SPOT_LIGHT_AMOUNT]   spotLightsShadowMaps;
uniform mat4 [MAX_SPOT_LIGHT_AMOUNT]        spotLightsProjections;
uniform mat4 [MAX_SPOT_LIGHT_AMOUNT]        spotLightsToLightTransformations;

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
        result += (1.0f - shadow) * count_light_influence(dirLight, diffuseColor, normalToUse_N, lightTransformationMatrix);
	}
	
    for(int j=0; j < spotLightsAmount; ++j) {
        vec4 fm = spotLightsProjections[j] * spotLightsToLightTransformations[j] * vec4(fragmentPositionWorld, 1.0f);
        
        float sh = calculate_pcf_shadow(spotLightsShadowMaps[0], fm, spotLights[j].direction, normalInWorld, 0.00000001f, 0.000001f);
        
        result += (1.0f - sh) * count_light_influence(spotLights[j],
                                                        fragmentPosition,
                                                        ambientColor,
                                                        diffuseColor,
                                                        specularColor,
                                                        shininess,
                                                        normalToUse_N,
                                                        cameraPosition,
                                                        surfaceZCamera_N,
                                                        lightTransformationMatrix);
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
                                        lightTransformationMatrix);
    }

	FragColor = vec4(result, 1.0f);
	
}

)";
