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

out vec4 FragColor;

in vec3 normalVector;

void main(){
	
	vec4 vertColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	
	if (hasDirLight == 1) {
		float angleCosine = count_diffuse_factor(normalVector, dirLight.direction);
		vec4 baseColor = vertColor * dirLight.color;
		FragColor = angleCosine * baseColor + 0.2 * vertColor;
		
	} else {
		
		FragColor = vertColor;
		
	}

}

)";
