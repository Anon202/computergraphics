#include <cstdlib>
#include "mesh.h"

float rnd() {
	return 2.0f * float(rand()) / float(RAND_MAX) - 1.0f;
}

void insertModel(Mesh **list, int nv, float * vArr, int nt, int * tArr, float scale) {
	Mesh * mesh = (Mesh *) malloc(sizeof(Mesh));
	mesh->nv = nv;
	mesh->nt = nt;	
	mesh->triangles = (Triangle *) malloc(nt * sizeof(Triangle));
    mesh->vertices = new vector<Vector>();
    mesh->vnorms = new vector<Vector>();

	// set mesh vertices
	for (int i = 0; i < nv; i++) {
        float x = vArr[i*3] * scale;
		float y = vArr[i*3+1] * scale;
		float z = vArr[i*3+2] * scale;
        mesh->vertices->push_back(Vector(x, y, z)); 
	}

	// set mesh triangles
	for (int i = 0; i < nt; i++) {
		mesh->triangles[i].vInds[0] = tArr[i*3];
		mesh->triangles[i].vInds[1] = tArr[i*3+1];
		mesh->triangles[i].vInds[2] = tArr[i*3+2];
	}

	// Assignment 1: 
	// Calculate and store suitable vertex normals for the mesh here.
	// Replace the code below that simply sets some arbitrary normal values	
	for (int i = 0; i < nv; i++) {
        mesh->vnorms->push_back(Vector(0, 0, 0));
    }
    for (int i = 0; i < nt; i++) {
        Triangle t = mesh->triangles[i];
        for (int j = 0; j < 3; j++) {
            Vector ab = mesh->vertices->at(t.vInds[(j+1)%3]) - mesh->vertices->at(t.vInds[j]);
            Vector ac = mesh->vertices->at(t.vInds[(j+2)%3]) - mesh->vertices->at(t.vInds[j]);
            mesh->vnorms->at(t.vInds[j]) += ab.cross(ac).normalized();
        }
    }
    for (int i = 0; i < nv; i++) {
        mesh->vnorms->at(i) = mesh->vnorms->at(i).normalized();
    }
	
    mesh->next = *list;
	*list = mesh;	
}
