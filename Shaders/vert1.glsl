#version 450 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 20) uniform mat4 vp;
layout (location = 21) uniform mat4 model;


void main(void){
    gl_Position = vp* model * vec4(position, 1.0f);
}