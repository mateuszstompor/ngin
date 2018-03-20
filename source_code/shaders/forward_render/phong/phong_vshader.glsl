R"(

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;

uniform mat4		 		modelTransformation;
uniform mat4 				cameraTransformation;
uniform mat4 				perspectiveProjection;

out vec2 texCoord;
out vec3 normalVector;
out vec3 fragmentPosition;

void main(){
	normalVector = normalize(transpose(inverse(mat3(modelTransformation))) * normal);
	vec4 pos = modelTransformation * vec4(position, 1.0f);
	fragmentPosition = pos.xyz;
	texCoord = textureCoordinates;
	gl_Position = perspectiveProjection * cameraTransformation * pos;
}

)";
