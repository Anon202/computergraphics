#version 450 core

uniform vec3 mAmbient;
uniform vec3 mDiffuse;
uniform vec3 mSpecular;
uniform float mShininess;

in vec3 vPos;
in vec3 vNorm;
uniform mat4 PVM;
out vec4 color;

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
    gl_Position = PVM * vec4(vPos, 1.0);

    vec3 n = vec3(PVM * vec4(vNorm, 0.0));
    vec4 v4 = PVM * vec4(vPos, 1.0);
    vec3 vposition = vec3(v4) / v4.w;
    vec3 acc_color = vec3(0, 0, 0);
    vec3 v = normalize(-vposition); 

    for (int i = 0; i < 2; i++) {
        vec3 l = normalize(lights[i].position - vposition);
        vec3 r = reflect(-l, n); 
        vec3 ambient = mAmbient * lights[i].ambient;
        vec3 diffuse = mDiffuse * lights[i].diffuse * max(dot(n, l), 0.0);
        vec3 specular = mSpecular * lights[i].specular * 
                    pow(max(dot(r, v), 0.0), mShininess);
        acc_color += ambient + diffuse + specular;
    }
    color = vec4(acc_color, 1.0);
}
