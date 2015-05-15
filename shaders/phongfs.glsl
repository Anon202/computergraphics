#version 450 core

in vec3 normalInterp;
in vec3 vposition;
out vec4 fColor;

struct LightData {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct MaterialData {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

LightData light0 = LightData(
    vec3(0.5, 0.0, 0.0),
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, 0.0, 0.0),
    vec3(1.0, 1.0, 1.0)
);

MaterialData material = MaterialData(
    vec3(0.3, 0.0, 0.0),
    vec3(0.5, 0.0, 0.0),
    vec3(1.0, 1.0, 1.0),
    5.0
);

void main(void) {
    vec3 v = normalize(-vposition);
    vec3 n = normalize(normalInterp);
    vec3 l = normalize(light0.position - vposition);
    vec3 r = reflect(-l, n);
    vec3 ambient = material.ambient * light0.ambient;
    vec3 diffuse = material.diffuse * light0.diffuse * max(dot(n, l), 0.0);
    vec3 specular = material.specular * light0.specular * 
                    pow(max(dot(r, v), 0.0), material.shininess);
    fColor = vec4(ambient + diffuse + specular, 1.0);
}
