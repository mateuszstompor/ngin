R"(

uniform	int									spotLightsAmount;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;

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

out vec4 FragColor;

in vec2 texCoord;

in vec3 normal_N;

in vec3 fragmentPosition;
in vec3 cameraPosition;
in vec3 surfaceZCamera_N;

in mat3 lightTransformationMatrix;

void main(){
	
	vec3 	ambientColor;
	vec3 	diffuseColor;
	vec3 	specularColor;
    vec3    normalToUse_N;
	float 	shininess;
	vec3 	result = vec3(0.0f);

	
	if(hasMaterial == 1) {
        ambientColor    = materialBlock.ambient;
		diffuseColor 	= hasDiffuseTexture == 1 ? texture(diffuseTexture, texCoord).xyz : materialBlock.diffuse;
		specularColor 	= hasSpecularTexture == 1 ? texture(specularTexture, texCoord).xyz : materialBlock.specular;
		shininess		= materialBlock.shininess;
	} else {
		// some default colors
        ambientColor    = vec3(1.0f, 0.0f, 0.0f);
        diffuseColor 	= vec3(1.0f, 0.0f, 0.0f);
		specularColor 	= vec3(1.0f, 0.0f, 0.0f);
		shininess		= 1.0f;
	}
    
//    if(hasNormalTexture == 1) {
//        normalToUse_N = normalize(texture(normalTexture, texCoord).xyz);
//        normalToUse_N = normalize(normalToUse_N * 2.0 - 1.0);
//        normalToUse_N = normalize(lightTransformationMatrix * normalToUse_N);
//    } else {
        normalToUse_N = normal_N;
//    }

	if (hasDirLight == 1) {
		result += count_light_influence(dirLight, diffuseColor, normalToUse_N);
	}
	
	for(int j=0; j < spotLightsAmount; ++j) {
		result += count_light_influence(spotLights[j],
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
