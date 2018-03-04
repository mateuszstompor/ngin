#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colors;

uniform mat4 cameraTranslation;
uniform mat4 perspectiveProjectionMatrix;



void main(){
    gl_Position = perspectiveProjectionMatrix * cameraTranslation * vec4(position, 1.0f);
}
