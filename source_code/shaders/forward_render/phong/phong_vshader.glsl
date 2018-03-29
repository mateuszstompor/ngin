R"(

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4		 		modelTransformation;
uniform mat4 				cameraTransformation;
uniform mat4 				perspectiveProjection;

out vec2 texCoord;
out vec3 normal_N;
out vec3 fragmentPosition;
out vec3 cameraPosition;
out vec3 surfaceZCamera_N;
out mat3 lightTransformationMatrix;

void main(){
    
    
    mat3 normalsTransformationMatrix = transpose(inverse(mat3(modelTransformation)));
    
    normal_N = normalize(normalsTransformationMatrix * normal);
//    vec3 tangent_N = normalize(normalsTransformationMatrix * tangent);
//    vec3 bitangent_N = normalize(normalsTransformationMatrix * bitangent);

//    mat3 toTangentSpace = mat3(
//        normal_N.x,     tangent_N.x,    bitangent_N.x,
//        normal_N.y,     tangent_N.y,    bitangent_N.y,
//        normal_N.z,     tangent_N.z,    bitangent_N.z
//    );

//    mat3 toTangentSpace = mat3(tangent_N, bitangent_N, normal_N);
    
	vec4 pos = modelTransformation * vec4(position, 1.0f);
	fragmentPosition = pos.xyz;
	cameraPosition = (cameraTransformation * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
	surfaceZCamera_N = normalize(cameraPosition - fragmentPosition);
	texCoord = textureCoordinates;
    lightTransformationMatrix = mat3(1.0f);
	gl_Position = perspectiveProjection * cameraTransformation * pos;
}

)";
