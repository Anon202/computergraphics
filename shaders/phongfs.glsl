#version 450 core

uniform vec3 mAmbient;
uniform vec3 mDiffuse;
uniform vec3 mSpecular;
uniform float mShininess;

in vec3 normalInterp;
in vec3 vposition;
out vec4 fColor;

struct LightData {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

LightData[2] lights = {
    LightData(
        vec3(1.0, 1.0, 1.0),
        vec3(1.0, 1.0, 1.0),
        vec3(1.0, 0.0, 0.0),
        vec3(1.0, 1.0, 1.0)
    ),
    LightData(
        vec3(0.0, 1.0, -1.0),
        vec3(1.0, 0.0, 0.0),
        vec3(1.0, 0.0, 1.0),
        vec3(0.0, 1.0, 0.0)
    )
};

void main(void) {
    vec3 v = normalize(-vposition);
    vec3 n = normalize(normalInterp);
    vec3 color = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < 2; i++) {
        vec3 l = normalize(lights[i].position - vposition);
        vec3 r = reflect(-l, n);
        vec3 ambient = mAmbient * lights[i].ambient;
        vec3 diffuse = mDiffuse * lights[i].diffuse * max(dot(n, l), 0.0);
        vec3 specular = mSpecular * lights[i].specular * 
                        pow(max(dot(r, v), 0.0), mShininess);
        color += ambient + diffuse + specular;
    }
    fColor = vec4(color, 1.0);
}
