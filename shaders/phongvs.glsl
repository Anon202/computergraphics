#version 450 core

uniform mat4 PVM;
in vec3 vPos;
in vec3 vNorm;
out vec3 normalInterp;
out vec3 vposition;

void main(void) {
    normalInterp = vec3(PVM * vec4(vNorm, 0.0));
    vec4 v4 = PVM * vec4(vPos, 1.0);
    vposition = vec3(v4) / v4.w;
    gl_Position = PVM * vec4(vPos, 1.0);
}
