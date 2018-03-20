R"(

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;

uniform mat4		 		modelTransformation;
uniform mat4 				cameraTransformation;
uniform mat4 				perspectiveProjection;

out vec3 normalVector;
out vec3 worldPosition;
out vec2 texCoord;

void main(){
	//in case of shearing
	normalVector 		= normalize(transpose(inverse(mat3(modelTransformation))) * normal);
	texCoord 			= textureCoordinates;
	worldPosition 		= (modelTransformation * vec4(position, 1.0f)).xyz;
	gl_Position 		= perspectiveProjection * cameraTransformation * modelTransformation * vec4(position, 1.0f);
}

)";
