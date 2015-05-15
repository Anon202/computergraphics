#version 450 core

in vec3 normalInterp;
in vec3 vposition;
out vec4 fColor;

vec3 light = vec3(1.0, 1.0, -1.0);

void main(void) {
    vec3 normal = normalize(normalInterp);
    vec3 vlight = normalize(light - vposition);
    float intensity = dot(vlight, normal); 
    if (intensity > 0.95)
        fColor = vec4(0.5, 0.5, 1.0, 1.0);
    else if (intensity > 0.75)
        fColor = vec4(0.4, 0.4, 0.8, 1.0);
    else if (intensity > 0.5)
        fColor = vec4(0.3, 0.3, 0.6, 1.0);
    else if (intensity > 0.25)
        fColor = vec4(0.2, 0.2, 0.4, 1.0);
    else
        fColor = vec4(0.1, 0.1, 0.2, 1.0);
}
