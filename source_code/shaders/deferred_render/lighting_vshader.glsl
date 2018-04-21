R"(
  
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 textureCoord;

uniform mat4 	cameraTransformation;

out vec2 		texCoords;
out vec3 		cameraPosition;

void main() {
	
	cameraPosition = (cameraTransformation * vec4(vec3(0.0f), 1.0f)).xyz;
	gl_Position = vec4(position, 1.0f);
	texCoords = textureCoord;
	
}
  
)";
