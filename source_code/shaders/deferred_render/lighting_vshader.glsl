R"(
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;

uniform mat4 	cameraTransformation;

out vec2 		texCoords;
out vec3 		cameraPosition;

void main() {
	
	cameraPosition = vec3(cameraTransformation[3][0], cameraTransformation[3][1], cameraTransformation[3][2]);
	gl_Position = vec4(position, 1.0f);
	texCoords = textureCoord;
	
}
  
)";
