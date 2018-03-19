R"(

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

uniform int 								hasMaterial;
uniform Material							material;

uniform int 								hasDiffuseTexture;
uniform sampler2D 							diffuseTexture;

uniform int 								hasSpecularTexture;
uniform sampler2D 							specularTexture;

in vec3		worldPosition;
in vec3		normalVector;
in vec2 	texCoord;

out vec4	FragColor;

void main(){
	
	if(hasMaterial == 1) {
		gAlbedo.xyz = hasDiffuseTexture == 1 ? texture(diffuseTexture, texCoord).xyz : material.diffuse;
		gAlbedo.w = hasSpecularTexture == 1 ? texture(specularTexture, texCoord).x : material.shininess;
	} else {
		gAlbedo = vec4(0.0f, 1.0f, 0.0f, 0.13f);
	}
	
	gNormal = normalVector;
	gPosition = worldPosition;
	
}

)";
