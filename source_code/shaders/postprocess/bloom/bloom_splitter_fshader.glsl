R"(

in vec2 texCoords;

layout (location = 1) out vec4 fragment;
layout (location = 0) out vec4 bright;

uniform int         isOn;
uniform sampler2D   in1;

void main() {
	
    if(isOn == 1) {
        vec4 color = texture(in1, texCoords);
        
        fragment = color;
        
        if (get_luminance(color.xyz) > 1.0f) {
            bright = 2.0f * vec4(color.rgb, 1.0f);
        } else {
            bright = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
    } else {
        fragment = texture(in1, texCoords);
        bright = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
	
}

)";
