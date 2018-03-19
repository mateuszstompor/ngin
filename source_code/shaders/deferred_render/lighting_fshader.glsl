R"(

in 		vec2								TexCoords;

uniform sampler2D 							gPosition;
uniform sampler2D 							gNormal;
uniform sampler2D 							gAlbedo;

uniform	int									spotLightsAmount;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;

uniform	int									pointLightsAmount;
uniform	PointLight [MAX_POINT_LIGHT_AMOUNT]	pointLights;

uniform	int 								hasDirLight;
uniform DirectionalLight 					dirLight;

uniform mat4 								cameraTransformation;

uniform uint								renderMode;

out 	vec4 								FragColor;

void main() {

#ifdef DEBUG

	if(renderMode > uint(0)) {
		if(renderMode == uint(1)) {
			FragColor = texture(gPosition, TexCoords);
			return;
		} else if (renderMode == uint(2)) {
			FragColor = texture(gAlbedo, TexCoords);
			return;
		} else if (renderMode == uint(3)) {
			FragColor = texture(gNormal, TexCoords);
			return;
		} else {
			float spec = texture(gAlbedo, TexCoords).w;
			FragColor = vec4(spec, spec, spec, 1.0f);
			return;
		}
	}

#endif

	
	vec3 normal = normalize(texture(gNormal, TexCoords).xyz);
	vec3 fragmentColor = texture(gAlbedo, TexCoords).xyz;
	vec3 fragmentPosition = texture(gPosition, TexCoords).xyz;
	
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraPosition = (cameraTransformation * vec4(1.0f)).xyz;
	vec3 surfaceToCamera = normalize(fragmentPosition - cameraPosition);
	
	result += 0.1f * texture(gAlbedo, TexCoords).xyz;
	
	if (hasDirLight == 1) {
		result += dirLight.color.xyz * fragmentColor * count_diffuse_factor(normal, dirLight.direction);
	}
	
	for(int j=0; j < spotLightsAmount; ++j) {
		
		vec3 lightToFragment = normalize(fragmentPosition - spotLights[j].position);
		float angle = max(dot(lightToFragment, spotLights[j].direction), 0.0f);
		float angleRadians = radians(spotLights[j].angleDegrees);
		if (angle > angleRadians) {
			vec3 lightPosition = spotLights[j].position;
			vec3 surfaceToLight = lightPosition - fragmentPosition;
			
			
			//use length2 instead of length
			float distance = length(surfaceToLight);
			
			surfaceToLight = normalize(surfaceToLight);
			
			float attenuation = count_attenuation_factor(distance);
			vec3 diffuseColor = spotLights[j].color.xyz * fragmentColor * count_diffuse_factor(normal, surfaceToLight) * attenuation;
			vec3 specularColor = spotLights[j].color.xyz * fragmentColor * count_blinn_phong_specular_factor(normalize(fragmentPosition - cameraPosition), surfaceToLight, normal, 256) * attenuation;
			
			result += diffuseColor + specularColor;
		}
		
	}

	for (int i = 0; i < pointLightsAmount; i++) {
		vec3 lightPosition = pointLights[i].position;
		vec3 surfaceToLight = lightPosition - fragmentPosition;
		
		
		//use length2 instead of length
		float distance = length(surfaceToLight);
		
		surfaceToLight = normalize(surfaceToLight);
		
		float attenuation = count_attenuation_factor(distance);
		vec3 diffuseColor = pointLights[i].color.xyz * fragmentColor * count_diffuse_factor(normal, surfaceToLight) * attenuation;
		vec3 specularColor = pointLights[i].color.xyz * fragmentColor * count_blinn_phong_specular_factor(normalize(fragmentPosition - cameraPosition), surfaceToLight, normal, 256) * attenuation;
		
		result += diffuseColor + specularColor;
	}

	FragColor = vec4(result, 1.0f);

}

)";
