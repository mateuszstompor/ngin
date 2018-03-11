
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

struct Material {
	vec3 	diffuse;
	vec3 	ambient;
	vec3 	specular;
	float 	shininess;
	float	opacity;
};

uniform int 								hasMaterial;
uniform Material							material;

in vec3		worldPosition;
in vec3		normalVector;

out vec4	FragColor;

void main(){
	
	if(hasMaterial == 1) {
		gAlbedo = vec4(material.diffuse, material.shininess);
	} else {
		gAlbedo = vec4(0.0f, 1.0f, 0.0f, 0.13f);
	}
	
	gNormal = normalVector;
	gPosition = worldPosition;
	
}
