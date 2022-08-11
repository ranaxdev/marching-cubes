#version 450 core

in vec3 vfragpos;
in vec3 vnormal;

layout (location = 22) uniform vec3 viewpos;
layout (location = 23) uniform vec3 lightpos;


out vec4 color;

void main(void){

    vec3 lightcol = vec3(1.0f, 1.0f, 1.0f);
    vec3 objectcol = vec3(0.0f, 0.0f, 1.0f);

    float ambient_s = 0.1;
    vec3 ambient = ambient_s * lightcol;

    vec3 norm = normalize(vnormal);
    vec3 lightdir = normalize(lightpos - vfragpos);
    float diff = max(dot(norm, lightdir), 0.0);
    vec3 diffuse = diff * lightcol;

    float spec_s = 1.5;
    vec3 viewDir = normalize(viewpos - vfragpos);
    vec3 reflectDir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec_s * spec * lightcol;

    vec3 result = (ambient + diffuse + specular) * objectcol;



    color = vec4(result, 1.0);
}