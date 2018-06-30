R"(

layout (location = 0) in vec3 position;

uniform mat4 toWorld;
uniform mat4 toLight;
uniform mat4 projection;

out float distance;

void main(){
    vec4 inLightPos = toLight * toWorld * vec4(position, 1.0f);
    gl_Position = projection * inLightPos;
    distance = length(inLightPos.xyz);
}

)";
