
#define MAX_SPOT_LIGHT_AMOUNT	20
#define MAX_POINT_LIGHT_AMOUNT	20

struct DirectionalLight {
	vec3    	direction;
	vec4    	color;
};

struct PointLight {
	float 		power;
	vec4		color;
	mat4		transformation;
};

struct SpotLight {
	float 		power;
	vec4 		color;
	mat4		transformation;
	float		angleDegrees;
	vec3		direction;
};

in 		vec2								TexCoords;

uniform sampler2D 							gPosition;
uniform sampler2D 							gNormal;
uniform sampler2D 							gAlbedo;

uniform	int									spotLightsAmount;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;

uniform	int									pointLightsAmount;
uniform	PointLight [MAX_POINT_LIGHT_AMOUNT]	pointLights;

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

	int i;

	vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	for (i = 0; i < pointLightsAmount; i++) {
		mat4 lightTransform = pointLights[i].transformation;
		vec3 lightPosition = vec3(lightTransform[3][0], lightTransform[3][1], lightTransform[3][2]);
		vec3 L = texture(gPosition, TexCoords).xyz - lightPosition;
		float dist = length(L);
		vec3 N = normalize(texture(gNormal, TexCoords).xyz);
		vec3 R = reflect(-L, N);
		float NdotR = max(0.0f, dot(N, R));
		float NdotL = max(0.0f, dot(N, L));
		float attenuation = 50.0f / (pow(dist, 2.0f) + 1.0f);
		vec3 diffuse_color = pointLights[i].color.xyz * texture(gAlbedo, TexCoords).xyz * NdotL * attenuation;
		vec3 specular_color = pointLights[i].color.xyz * pow(NdotR, texture(gAlbedo, TexCoords).w) * attenuation;
		result += vec4(0.1f * texture(gAlbedo, TexCoords).xyz + diffuse_color/* + 0.specular_color*/, 0.0f);
	}

	FragColor = result;

}
