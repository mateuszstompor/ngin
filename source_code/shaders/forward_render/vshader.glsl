//#version 300 es
//precision highp float;

#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;

uniform mat4 cameraTranslation;
uniform mat4 perspectiveProjectionMatrix;

void main(){
    gl_Position = perspectiveProjectionMatrix * cameraTranslation * vec4(position, 1.0f);
}
