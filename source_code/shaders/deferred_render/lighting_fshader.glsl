R"(

in 		vec2								texCoords;
in 		vec3 								cameraPosition;

uniform sampler2D 							gPosition;
uniform sampler2D 							gNormal;
uniform sampler2D 							gAlbedo;

uniform	int									spotLightsAmount;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;

uniform	int									pointLightsAmount;
uniform	PointLight [MAX_POINT_LIGHT_AMOUNT]	pointLights;

uniform	int 								hasDirLight;
uniform DirectionalLight 					dirLight;

uniform uint								renderMode;

out 	vec4 								FragColor;

void main() {

#ifdef DEBUG

	if(renderMode > uint(0)) {
		if(renderMode == uint(1)) {
			FragColor 	= texture(gPosition, texCoords);
			return;
		} else if (renderMode == uint(2)) {
			FragColor 	= texture(gAlbedo, texCoords);
			return;
		} else if (renderMode == uint(3)) {
			FragColor 	= texture(gNormal, texCoords);
			return;
		} else {
			float spec 	= texture(gAlbedo, texCoords).w;
			FragColor 	= vec4(spec, spec, spec, 1.0f);
			return;
		}
	}

#endif

	vec4 color 				= texture(gAlbedo, texCoords);
	vec3 normal_N 			= texture(gNormal, texCoords).xyz;
	vec3 fragmentPosition 	= texture(gPosition, texCoords).xyz;
	vec3 diffuseColor		= color.xyz;
	vec3 specularColor		= vec3(color.w, color.w, color.w);
	
	vec3 result 			= vec3(0.0f, 0.0f, 0.0f);
	
	vec3 surfaceZCamera_N 	= normalize(cameraPosition - fragmentPosition);
	float shininess 		= 32.0f;
	
	if (hasDirLight == 1) {
		result += count_light_influence(dirLight, diffuseColor, normal_N);
	}
	
	for(int j=0; j < spotLightsAmount; ++j) {
		result += count_light_influence(spotLights[j],
										fragmentPosition,
										diffuseColor,
										diffuseColor,
										specularColor,
										shininess,
										normal_N,
										cameraPosition,
										surfaceZCamera_N,
                                        mat3(1.0f));
	}

	for (int i = 0; i < pointLightsAmount; ++i) {
		result += count_light_influence(pointLights[i],
										fragmentPosition,
										diffuseColor,
										diffuseColor,
										specularColor,
										shininess,
										normal_N,
										cameraPosition,
										surfaceZCamera_N,
                                        mat3(1.0f));
	}

	FragColor = vec4(result, 1.0f);

}

)";
