#version 410 core
//#version 300 es
//precision highp float;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position, 1.0);
    TexCoords = textureCoord;
}
