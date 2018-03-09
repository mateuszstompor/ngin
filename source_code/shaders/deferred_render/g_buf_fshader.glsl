#version 410 core
//#version 300 es
//precision highp float;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

struct DirectionalLight {
	vec3    	direction;
	vec4    	color;
};

struct Material {
	vec3 	diffuse;
	vec3 	ambient;
	vec3 	specular;
	float 	shininess;
	float	opacity;
};

uniform int 				hasMaterial;
uniform Material			material;
uniform	int 				hasDirLight;
uniform DirectionalLight 	dirLight;

in vec3 worldPosition;
in vec3 normalVector;

out vec4 FragColor;

void main(){
	
	gAlbedo = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	gNormal = normalVector;
	gPosition = worldPosition;
	
}
