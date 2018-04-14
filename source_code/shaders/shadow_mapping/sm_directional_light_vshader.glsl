R"(

layout (location = 0) in vec3 position;

uniform mat4		 		modelTransformation;
uniform mat4 				cameraTransformation;
uniform mat4 				projection;


void main(){
    mat3 normalsTransformationMatrix = transpose(inverse(mat3(modelTransformation)));
    gl_Position = projection * cameraTransformation * modelTransformation * vec4(position, 1.0f);
}

)";
