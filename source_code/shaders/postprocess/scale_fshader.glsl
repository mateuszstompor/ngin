R"(

in vec2 texCoords;

layout (location = 0) out vec4 fragment;

uniform sampler2D in1;

void main() {
	
	fragment = texture(in1, texCoords);
		
}

)";

