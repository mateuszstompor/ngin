R"(

uniform	int									spotLightsAmount;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;

uniform	int									pointLightsAmount;
uniform	PointLight [MAX_POINT_LIGHT_AMOUNT]	pointLights;

uniform	int 								hasDirLight;
uniform DirectionalLight 					dirLight;

uniform int 								hasMaterial;
uniform Material							material;

uniform int 								hasDiffuseTexture;
uniform sampler2D 							diffuseTexture;

uniform int 								hasSpecularTexture;
uniform sampler2D 							specularTexture;

uniform mat4 								cameraTransformation;

out vec4 FragColor;

in vec2 texCoord;
in vec3 normalVector;
in vec3 fragmentPosition;

void main(){
	
	vec4 fragmentBaseColor;
	vec3 result = vec3(0.0f);
	
	if(hasMaterial == 1) {
		fragmentBaseColor.xyz = hasDiffuseTexture == 1 ? texture(diffuseTexture, texCoord).xyz : material.diffuse;
		fragmentBaseColor.w = hasSpecularTexture == 1 ? texture(specularTexture, texCoord).x : material.shininess;
	} else {
		fragmentBaseColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	
	vec3 normal 			= normalize(normalVector);
	vec3 cameraPosition 	= (cameraTransformation * vec4(1.0f)).xyz;

	if (hasDirLight == 1) {
		result += count_light_influence(dirLight, fragmentBaseColor, normal).xyz;
	}

	for(int j=0; j < spotLightsAmount; ++j) {
		result += count_light_influence(spotLights[j], fragmentPosition, fragmentBaseColor, normal, cameraPosition).xyz;
	}

	for (int i = 0; i < pointLightsAmount; i++) {
		result += count_light_influence(pointLights[i], fragmentPosition, fragmentBaseColor, normal, cameraPosition).xyz;
	}

	FragColor = vec4(result, 1.0f);
	
}

)";
