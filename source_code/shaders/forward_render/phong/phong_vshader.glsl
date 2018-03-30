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
out mat4 lightTransformationMatrix;

//tangent
//void main(){
//
//
//    mat3 normalsTransformationMatrix = transpose(inverse(mat3(cameraTransformation * modelTransformation)));
//
//    vec3 n = normalize(normalsTransformationMatrix * normal);
//    vec3 tangent_N = normalize(normalsTransformationMatrix * tangent);
//    vec3 bitangent_N = normalize(normalsTransformationMatrix * bitangent);
//
//    normal_N = n;
//
//    mat4 tangentSpace = mat4(
//        tangent_N.x,     bitangent_N.x,    n.x,  0.0f,
//        tangent_N.y,     -bitangent_N.y,    n.y,  0.0f,
//        tangent_N.z,     bitangent_N.z,    n.z,  0.0f,
//        0.0f,            0.0f,             0.0f,        1.0f
//    );
//
//    vec4 pos = modelTransformation * vec4(position, 1.0f);
//    fragmentPosition = (tangentSpace * cameraTransformation * pos).xyz;
//    cameraPosition = (tangentSpace * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
//    surfaceZCamera_N = normalize(cameraPosition - fragmentPosition);
//    texCoord = textureCoordinates;
//    lightTransformationMatrix = tangentSpace * cameraTransformation;
//    gl_Position = perspectiveProjection * cameraTransformation * pos;
//}

//view
//void main(){
//
//    normal_N = normalize(transpose(inverse(mat3(cameraTransformation * modelTransformation))) * normal);
//
//    vec4 pos = modelTransformation * vec4(position, 1.0f);
//    fragmentPosition = (cameraTransformation * pos).xyz;
//    cameraPosition = vec3(0.0f);
//    surfaceZCamera_N = normalize(cameraPosition - fragmentPosition);
//    texCoord = textureCoordinates;
//    lightTransformationMatrix = cameraTransformation;
//    gl_Position = perspectiveProjection * cameraTransformation * pos;
//}


//world
void main(){


    mat3 normalsTransformationMatrix = transpose(inverse(mat3(modelTransformation)));

    normal_N = normalize(normalsTransformationMatrix * normal);

    vec4 pos = modelTransformation * vec4(position, 1.0f);
    fragmentPosition = pos.xyz;
    cameraPosition = (cameraTransformation * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
    surfaceZCamera_N = normalize(cameraPosition - fragmentPosition);
    texCoord = textureCoordinates;
    lightTransformationMatrix = mat4(1.0f);
    gl_Position = perspectiveProjection * cameraTransformation * pos;

}

)";
