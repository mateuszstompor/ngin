R"(

in 		vec2		texCoords;

out 	vec4 		out0;

uniform int         isOn;
uniform sampler2D 	in0;
uniform sampler2D 	in1;

void main() {
    if (isOn == 1) {
        out0 = texture(in0, texCoords) + texture(in1, texCoords);
    } else {
        out0 = texture(in1, texCoords);
    }
}

)";
