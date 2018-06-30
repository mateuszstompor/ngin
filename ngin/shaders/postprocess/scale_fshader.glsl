R"(

in vec2 texCoords;

layout (location = 0) out vec4 fragment;

uniform sampler2D in0;



void main() {
	
	fragment = texture(in0, texCoords);

}

)";

