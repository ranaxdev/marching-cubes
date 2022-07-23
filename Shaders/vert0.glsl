#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

layout (location = 20) uniform mat4 vp;
layout (location = 21) uniform mat4 model;

out vec4 vcolor;
out vec3 vfragpos;
out vec3 vnormal;

void main(void){
    gl_Position = vp*  model * vec4(position, 1.0f);

    vcolor = vec4(color, 1.0f);
    vfragpos = vec3(model * vec4(position, 1.0f));
    vnormal = mat3(transpose(inverse(model))) * normal;
}