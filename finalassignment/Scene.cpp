#include "Scene.h"

Scene::Scene(void) {
}

void Scene::Add(const Sphere& s) {
    this->spheres.push_back(s);
}

void Scene::Load(char *fileName) {
}
