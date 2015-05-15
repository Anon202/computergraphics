#version 450 core

uniform mat4 PVM;
uniform mat4 VM;
in vec3 vPos;
in vec3 vNorm;
out vec3 normalInterp;
out vec3 vposition;

void main(void) {
    vec4 v4 = PVM * vec4(vPos, 1.0);
    vposition = vec3(v4) / v4.w;
    normalInterp = vec3(PVM * vec4(vNorm, 0.0));
    gl_Position = PVM * vec4(vPos, 1.0);
}
