#version 410 core
//#version 300 es
//precision highp float;

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

uniform	uint								spotLightsAmount;
uniform	SpotLight [MAX_SPOT_LIGHT_AMOUNT] 	spotLights;

uniform	uint								pointLightsAmount;
uniform	PointLight [MAX_POINT_LIGHT_AMOUNT]	pointLights;

out 	vec4 								FragColor;

void main() {

	FragColor = texture(gPosition, TexCoords);

}
