R"(

in 		vec2		texCoords;
out 	vec4 		FragColor;

uniform sampler2D 	passedtexture;

void main() {
	FragColor = texture(passedtexture, texCoords);
}

)";
