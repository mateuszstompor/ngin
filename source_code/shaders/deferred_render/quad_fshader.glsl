#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;

void main() {

	FragColor = texture(gPosition, TexCoords);
	
//    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

}
