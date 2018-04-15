R"(

layout (location = 0) in vec3 position;

uniform mat4 toWorld;
uniform mat4 toLight;
uniform mat4 projection;

void main(){
    gl_Position = projection * toLight * toWorld * vec4(position, 1.0f);
}

)";
