R"(

out vec4 FragColor;

uniform vec3 lightSourceColor;

void main(void){
	//I multiply to make sure that they are the brightest spots in scene
	FragColor = vec4(4.0f * lightSourceColor, 1.0f);
}

)";
