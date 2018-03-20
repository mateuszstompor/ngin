R"(

out vec4 FragColor;

uniform vec3 lightSourceColor;

void main(void){
	FragColor = vec4(lightSourceColor,1.0f);
}

)";
