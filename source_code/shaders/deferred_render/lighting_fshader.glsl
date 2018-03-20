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
			FragColor 	= texture(gPosition, TexCoords);
			return;
		} else if (renderMode == uint(2)) {
			FragColor 	= texture(gAlbedo, TexCoords);
			return;
		} else if (renderMode == uint(3)) {
			FragColor 	= texture(gNormal, TexCoords);
			return;
		} else {
			float spec 	= texture(gAlbedo, TexCoords).w;
			FragColor 	= vec4(spec, spec, spec, 1.0f);
			return;
		}
	}

#endif

	vec3 normal 			= normalize(texture(gNormal, TexCoords).xyz);
	vec3 fragmentColor		= texture(gAlbedo, TexCoords).xyz;
	vec3 fragmentPosition 	= texture(gPosition, TexCoords).xyz;
	
	vec3 result 			= vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraPosition 	= (cameraTransformation * vec4(1.0f)).xyz;
	
	if (hasDirLight == 1) {
		result += count_light_influence(dirLight, vec4(fragmentColor, 1.0f), normal).xyz;
	}
	
	for(int j=0; j < spotLightsAmount; ++j) {
		result += count_light_influence(spotLights[j], fragmentPosition, vec4(fragmentColor, 1.0f), normal, cameraPosition).xyz;
	}

	for (int i = 0; i < pointLightsAmount; i++) {
		result += count_light_influence(pointLights[i], fragmentPosition, vec4(fragmentColor, 1.0f), normal, cameraPosition).xyz;
	}

	FragColor = vec4(result, 1.0f);

}

)";
