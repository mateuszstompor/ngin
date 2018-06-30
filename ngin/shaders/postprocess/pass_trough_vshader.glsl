R"(

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 textureCoord;

out vec2 		texCoords;

void main() {
	
	gl_Position = vec4(position, 1.0f);
	texCoords = textureCoord;
	
}

)";

