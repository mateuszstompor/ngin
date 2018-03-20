R"(

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normalVector;
layout (location = 2) in vec2 textureCoordinates;

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

uniform mat4		 						modelTransformation;
uniform mat4 								cameraTransformation;
uniform mat4 								perspectiveProjection;

out 	vec4 								Fragmentcolor;

void main(){
	
	vec4 worldPosition = modelTransformation * vec4(position, 1.0f);
	
	vec4 fragmentBaseColor;
	vec3 result = vec3(0.0f);
	
	if(hasMaterial == 1) {
		fragmentBaseColor.xyz = hasDiffuseTexture == 1 ? texture(diffuseTexture, textureCoordinates).xyz : material.diffuse;
		fragmentBaseColor.w = hasSpecularTexture == 1 ? texture(specularTexture, textureCoordinates).x : material.shininess;
	} else {
		fragmentBaseColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	
	vec3 normal 			= normalize(transpose(inverse(mat3(modelTransformation))) * normalVector);
	vec3 cameraPosition 	= (cameraTransformation * vec4(1.0f)).xyz;
	
	if (hasDirLight == 1) {
		result += count_light_influence(dirLight, fragmentBaseColor, normal).xyz;
	}
	
	for(int j = 0; j < spotLightsAmount; ++j) {
		result += count_light_influence(spotLights[j], worldPosition.xyz, fragmentBaseColor, normal, cameraPosition).xyz;
	}

	for (int i = 0; i < pointLightsAmount; i++) {
		result += count_light_influence(pointLights[i], worldPosition.xyz, fragmentBaseColor, normal, cameraPosition).xyz;
	}

	vec4 FragColor = vec4(result, 1.0f);
	
	Fragmentcolor = FragColor;
	gl_Position = perspectiveProjection * cameraTransformation * worldPosition;
}

)";
