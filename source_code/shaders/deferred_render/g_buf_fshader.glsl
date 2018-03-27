R"(

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

uniform int hasMaterial;

layout (std140) uniform  MaterialBlock {
	vec3 	ambient;
	vec3 	diffuse;
	vec3 	specular;
	float 	shininess;
	float	opacity;
} materialBlock;

uniform int 								hasDiffuseTexture;
uniform sampler2D 							diffuseTexture;

uniform int 								hasSpecularTexture;
uniform sampler2D 							specularTexture;

in 		vec3								worldPosition;
in 		vec3								normalVector;
in 		vec2 								texCoord;

void main(){
	
	if(hasMaterial == 1) {
		gAlbedo.xyz = hasDiffuseTexture == 1 ? texture(diffuseTexture, texCoord).xyz : materialBlock.diffuse;
		gAlbedo.w = hasSpecularTexture == 1 ? texture(specularTexture, texCoord).x : materialBlock.shininess;
	} else {
		gAlbedo = vec4(0.0f, 1.0f, 0.0f, 0.5f);
	}
	
	gNormal = normalVector;
	gPosition = worldPosition;
	
}

)";
