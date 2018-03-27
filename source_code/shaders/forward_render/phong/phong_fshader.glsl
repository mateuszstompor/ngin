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

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal_N;
in vec3 fragmentPosition;
in vec3 cameraPosition;
in vec3 surfaceZCamera_N;

void main(){
	
	vec3 	ambientColor;
	vec3 	diffuseColor;
	vec3 	specularColor;
	float 	shininess;
	vec3 	result = vec3(0.0f);

	ambientColor = materialBlock.ambient;
	
	if(hasMaterial == 1) {
		diffuseColor 	= hasDiffuseTexture == 1 ? texture(diffuseTexture, texCoord).xyz : materialBlock.diffuse;
		specularColor 	= hasSpecularTexture == 1 ? texture(specularTexture, texCoord).xyz : materialBlock.specular;
		shininess		= materialBlock.shininess;
	} else {
		// some default colors
		diffuseColor 	= vec3(1.0f, 1.0f, 0.0f);
		specularColor 	= vec3(1.0f, 0.0f, 0.0f);
		shininess		= 0.0f;
	}

	if (hasDirLight == 1) {
		result += count_light_influence(dirLight, diffuseColor, normal_N);
	}
	
	for(int j=0; j < spotLightsAmount; ++j) {
		result += count_light_influence(spotLights[j],
										fragmentPosition,
										ambientColor,
										diffuseColor,
										specularColor,
										shininess,
										normal_N,
										cameraPosition,
										surfaceZCamera_N);
	}
	
	for (int i = 0; i < pointLightsAmount; ++i) {
		result += count_light_influence(pointLights[i],
										fragmentPosition,
										ambientColor,
										diffuseColor,
										specularColor,
										shininess,
										normal_N,
										cameraPosition,
										surfaceZCamera_N);
	}

	FragColor = vec4(result, 1.0f);
	
}

)";
