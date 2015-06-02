#include "Shape.h"

Shape::Shape(Material mat) : material(mat) {
}

Color Shape::GetColor(Vector position) const {
    return this->material.ambient;
}
