#version 410 core
//#version 300 es
//precision highp float;

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

void main() {

	FragColor = texture(gPosition, TexCoords);

}
