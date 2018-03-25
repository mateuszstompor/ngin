R"(

in 		vec2		texCoords;

out 	vec4 		out0;

uniform sampler2D 	in0;
uniform sampler2D 	in1;

void main() {
	out0 = texture(in1, texCoords) + texture(in1, texCoords);
}

)";
