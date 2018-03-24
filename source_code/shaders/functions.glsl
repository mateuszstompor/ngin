R"(

#define A_PARAM 					0.1f
#define B_PARAM 					0.01f
#define C_PARAM						1.0f
#define MIN_ATTENUATION				0.01f

#define AMBIENT_STRENGTH			0.05f
#define DIFFUSE_STRENGTH			0.9f
#define SPECULAR_STRENGTH			0.8f

#define SPOT_LIGHT_INNER_CUTOFF		0.50f
#define SPOT_LIGHT_OUTER_CUTOFF		0.91f

#define RED_VALUE_WEIGHT			0.2126
#define GREEN_VALUE_WEIGHT			0.7152
#define BLUE_VALUE_WEIGHT			0.0722

struct DirectionalLight {
	vec3    	direction;
	vec3    	color;
};

struct PointLight {
	float 		power;
	vec3		color;
	vec3		position;
};

struct SpotLight {
	float 		power;
	vec3 		color;
	vec3		position;
	float		angleDegrees;
	vec3		direction;
};


					//////////////////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////////////
					//																					//
					//	I had to find a way how to indicate in which side of direction vector points,	//
					//	so I decided to do this in such way												//
					//	We are given vec A and B														//
					//	I mark subtraction A - B as "ZAB" which means that it is a vector,				//
					//	which was created as a product of subtraction two vectors and points to A		//
					//																					//
					//	I mark normalized vectors with "_N" at the end of name							//
					//////////////////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////////////

float count_diffuse_factor(vec3 normal_N, vec3 surfaceZCamera_N) {
	return max(dot(normal_N, surfaceZCamera_N), 0.0f);
}

float count_phong_specular_factor(vec3 surfaceZCamera_N, vec3 surfaceZLight_N, vec3 normal_N, float shininess) {
	return pow(max(0.0f, dot(surfaceZCamera_N, reflect(-surfaceZLight_N, normal_N))), shininess);
}

float count_blinn_phong_specular_factor(vec3 surfaceZCamera_N, vec3 surfaceZLight_N, vec3 normal_N, float shininess) {
	vec3 H = normalize(surfaceZCamera_N + surfaceZLight_N);
	return pow(max(0.0f, dot(H, normal_N)), shininess);
}

float count_attenuation_factor(float distance) {
	return 1.0f/(B_PARAM * distance * distance + A_PARAM * distance + C_PARAM);
}

vec3 count_light_influence(PointLight 	light,
						   vec3 		surfacePosition,
						   vec3 		ambientColor,
						   vec3 		diffuseColor,
						   vec3 		specularColor,
						   float		shininess,
						   vec3 		normal_N,
						   vec3 		cameraPosition,
						   vec3 		fragmentZCamera_N,
						   vec3			surfaceZLight,
						   vec3			surfaceZLight_N) {
	
	
	//TODO USE LENGTH2 INSTEAD OF LENGTH
	float distance = length(surfaceZLight);
		
	float attenuation = count_attenuation_factor(distance);
	
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	
	result += light.color.xyz * ambientColor * attenuation * AMBIENT_STRENGTH;
	
	if(attenuation > MIN_ATTENUATION) {
		result += light.color.xyz * diffuseColor * count_diffuse_factor(normal_N, surfaceZLight_N) * attenuation * DIFFUSE_STRENGTH;
		result += light.color.xyz * specularColor *
		count_blinn_phong_specular_factor(fragmentZCamera_N, surfaceZLight_N, normal_N, shininess) * attenuation * SPECULAR_STRENGTH;
	}
	
	return result;
	
}

vec3 count_light_influence(PointLight 	light,
						   vec3 		surfacePosition,
						   vec3 		ambientColor,
						   vec3 		diffuseColor,
						   vec3 		specularColor,
						   float		shininess,
						   vec3 		normal_N,
						   vec3 		cameraPosition,
						   vec3 		fragmentZCamera_N) {
	
	vec3 surfaceZLight = light.position - surfacePosition;
	vec3 surfaceZLight_N = normalize(surfaceZLight);
	return count_light_influence(light,
								 surfacePosition,
								 ambientColor,
								 diffuseColor,
								 specularColor,
								 shininess,
								 normal_N,
								 cameraPosition,
								 fragmentZCamera_N,
								 surfaceZLight,
								 surfaceZLight_N);
	
}

vec3 count_light_influence(DirectionalLight light, vec3 diffuseColor, vec3 normal_N) {
	return light.color.xyz * diffuseColor * count_diffuse_factor(normal_N, light.direction) * DIFFUSE_STRENGTH;
}

vec3 count_light_influence(SpotLight 	light,
						   vec3 		surfacePosition,
						   vec3 		ambientColor,
						   vec3 		diffuseColor,
						   vec3 		specularColor,
						   float		shininess,
						   vec3 		normal_N,
						   vec3 		cameraPosition,
						   vec3 		fragmentZCamera_N) {

	vec3 lightZFragment 	= surfacePosition - light.position;
	vec3 lightZFragment_N	= normalize(lightZFragment);
	
	float angleRadians = radians(max(dot(lightZFragment, light.direction), 0.0f));

	vec3 color = count_light_influence(PointLight(light.power, light.color, light.position),
									   surfacePosition,
									   ambientColor,
									   diffuseColor,
									   specularColor,
									   shininess,
									   normal_N,
									   cameraPosition,
									   fragmentZCamera_N,
									   -lightZFragment,
									   -lightZFragment_N);

	float epsilon   = SPOT_LIGHT_OUTER_CUTOFF - SPOT_LIGHT_INNER_CUTOFF;
	float intensity = clamp((dot(lightZFragment_N, normalize(light.direction)) - SPOT_LIGHT_INNER_CUTOFF) / epsilon, 0.0f, 1.0f);

	return intensity * color;

}

float get_luminance(vec3 color) {
	return dot(color, vec3(RED_VALUE_WEIGHT, GREEN_VALUE_WEIGHT, BLUE_VALUE_WEIGHT));
}

)";
