R"(

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

)";
