R"(

in 		vec2		texCoords;

out 	vec4 		out0;

uniform sampler2D 	passedtexture;

void main() {
	out0 = texture(passedtexture, texCoords);
}

)";
