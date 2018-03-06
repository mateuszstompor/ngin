#version 410 core
//#version 300 es
//precision highp float;

struct DirectionalLight {
	vec3    	direction;
	vec4    	color;
	float    	power;
};

uniform	int 				hasDirLight;
uniform DirectionalLight 	dirLight;

out vec4 FragColor;

smooth in vec3 normalVector;

void main(){
	
	vec4 vertColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	
	if (hasDirLight == 1) {
		
		float angleCosine = max(dot(normalize(normalVector), normalize(dirLight.direction)), 0.0);
		vec4 baseColor = vertColor * dirLight.color;
		FragColor = angleCosine * baseColor + 0.2 * vertColor;
		
	} else {
		
		FragColor = vertColor;
		
	}

}
