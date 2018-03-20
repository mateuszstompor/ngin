R"(

struct DirectionalLight {
	vec3    	direction;
	vec4    	color;
};

struct PointLight {
	float 		power;
	vec4		color;
	vec3		position;
};

struct SpotLight {
	float 		power;
	vec4 		color;
	vec3		position;
	float		angleDegrees;
	vec3		direction;
};

#define A_PARAM 0.1
#define B_PARAM 0.01

float count_diffuse_factor(vec3 normalVector, vec3 surfaceToLight) {
	return max(dot(normalize(normalVector), normalize(surfaceToLight)), 0.0);
}

float count_phong_specular_factor(vec3 surfaceToCamera, vec3 surfaceToLight, vec3 normal, float shininess) {
	return pow(max(0.0, dot(normalize(surfaceToCamera), normalize(reflect(-surfaceToLight, normal)))), shininess);
}

float count_blinn_phong_specular_factor(vec3 surfaceToCamera, vec3 surfaceToLight, vec3 normal, float shininess) {
	vec3 H = normalize(surfaceToCamera + surfaceToLight);
	return pow(max(0.0, dot(H, normal)), shininess);
}

float count_attenuation_factor(float distance) {
	return 1.0f/(1.0f + A_PARAM * distance + B_PARAM * distance * distance);
}

vec4 count_light_influence(PointLight light, vec3 fragmentPosition, vec4 fragmentColor, vec3 normalVector, vec3 cameraPosition) {
	
	vec3 surfaceToLight = light.position - fragmentPosition;
	
	//TODO USE LENGTH2 INSTEAD OF LENGTH
	float distance = length(surfaceToLight);
	
	surfaceToLight = normalize(surfaceToLight);
	
	float attenuation = count_attenuation_factor(distance);
	vec3 fragmentToCamera = normalize(fragmentPosition - cameraPosition);
	vec3 diffuseColor = light.color.xyz * fragmentColor.xyz * count_diffuse_factor(normalVector, surfaceToLight) * attenuation;
	vec3 specularColor = light.color.xyz * fragmentColor.xyz * count_blinn_phong_specular_factor(fragmentToCamera, surfaceToLight, normalVector, 256) * attenuation;
	vec3 ambientColor = light.color.xyz * fragmentColor.xyz * attenuation * 0.1;
	
	return vec4(diffuseColor + specularColor, 1.0f);
}

vec4 count_light_influence(DirectionalLight light, vec4 fragmentColor, vec3 normalVector) {
	return light.color * fragmentColor * count_diffuse_factor(normalVector, light.direction);
}

vec4 count_light_influence(SpotLight light, vec3 fragmentPosition, vec4 fragmentColor, vec3 normalVector, vec3 cameraPosition) {
	
	vec3 lightToFragment = normalize(fragmentPosition - light.position);
	float angle = max(dot(lightToFragment, light.direction), 0.0f);
	float angleRadians = radians(light.angleDegrees);

	vec4 col = count_light_influence(PointLight(light.power, light.color, light.position), fragmentPosition, fragmentColor, normalVector, cameraPosition);

	float epsilon   = 0.91 - 0.50;
	float intensity = clamp((dot(normalize(lightToFragment), normalize(light.direction)) - 0.50f) / epsilon, 0.0, 1.0);
	
	return vec4(intensity * col.xyz, 1.0f);
	
}


)";
