
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4		 		modelTransformation;
uniform mat4 				cameraTransformation;
uniform mat4 				perspectiveProjection;

out vec3 normalVector;

void main(){
	normalVector = normal;
    gl_Position = perspectiveProjection * cameraTransformation * modelTransformation * vec4(position, 1.0f);
}
