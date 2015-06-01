#include "Scene.h"

Scene::Scene(void) {
}

void Scene::Add(Shape* s) {
    this->shapes.push_back(s);
}

void Scene::Add(const Light& s) {
    this->lights.push_back(s);
}

void Scene::Load(char *fileName) {
}
