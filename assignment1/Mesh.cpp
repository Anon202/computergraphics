#include <cstdio>
#include <cstring>
#include <cmath>
#include "Mesh.h"

Mesh::Mesh(int nv, int nt, float *vArr, int *tArr) : nv(nv), nt(nt) {
    this->vertices.resize(nv);
    this->vnorms.resize(nv);
    this->triangles.resize(nt);
    this->scale = Vector(1, 1, 1);
    this->rotation = Vector(0, 0, 0);
    this->translation = Vector(0, 0, 0);
	
    // set mesh vertices
	for (int i = 0; i < nv; i++) {
        float x = vArr[i*3];
		float y = vArr[i*3+1];
		float z = vArr[i*3+2];
        this->vertices[i] = Vector(x, y, z); 
	}

	// set mesh triangles
	for (int i = 0; i < nt; i++) {
		this->triangles[i].vInds[0] = tArr[i*3];
		this->triangles[i].vInds[1] = tArr[i*3+1];
		this->triangles[i].vInds[2] = tArr[i*3+2];
	}

	// Assignment 1: 
	// Calculate and store suitable vertex normals for the mesh here.
	// Replace the code below that simply sets some arbitrary normal values	
    for (int i = 0; i < nt; i++) {
        Triangle t = this->triangles[i];
        Vector ab = this->vertices[t.vInds[1]] - this->vertices[t.vInds[0]];
        Vector ac = this->vertices[t.vInds[2]] - this->vertices[t.vInds[0]];
        for (int j = 0; j < 3; j++) {
            this->vnorms[t.vInds[j]] += ab.cross(ac).normalized();
        }
    }
    for (int i = 0; i < nv; i++) {
        this->vnorms[i] = this->vnorms[i].normalized();
    }

}

int Mesh::nvertices() {
    return this->nv;
}

int Mesh::ntriangles() {
    return this->nt;
}

void Mesh::setScale(Vector scale) {
    this->scale = scale;
}

void Mesh::setRotation(Vector rotation) {
    this->rotation = rotation;
}

void Mesh::setTranslation(Vector translation) {
    this->translation = translation;
}

Matrix Mesh::transformationMatrix() {
    return Matrix::translation(translation)
            * Matrix::rotation('x', rotation.x)
            * Matrix::rotation('y', rotation.y)
            * Matrix::rotation('z', rotation.z)
            * Matrix::scale(scale);
}

void Mesh::Move(char dir) {
    int f = 1;
    if (dir < 'A' || dir > 'Z') {
        f = -1;
    }
    switch (dir) {
        case 'x': case 'X':
            this->translation.x += f*0.2;
            break;
        case 'y': case 'Y':
            this->translation.y += f*0.2;
            break;
        case 'z': case 'Z':
            this->translation.z += f*0.2;
            break;
        default:
            // TODO: throw exception
            break;
    }
}

void Mesh::Rotate(char dir) {
    int f = 1;
    if (dir < 'A' || dir > 'Z') {
        f = -1;
    }
    switch (dir) {
        case 'i': case 'I':
            this->rotation.x += f*M_PI/100;
            break;
        case 'j': case 'J':
            this->rotation.y += f*M_PI/100;
            break;
        case 'k': case 'K':
            this->rotation.z += f*M_PI/100;
            break;
        default:
            // TODO: throw exception
            break;
    }
}

void Mesh::Scale(char dir) {
    int f = 1;
    if (dir < 'A' || dir > 'Z') {
        f = -1;
    }
    switch (dir) {
        case 'x': case 'X':
            this->scale.x += f*0.02;
            break;
        case 'y': case 'Y':
            this->scale.y += f*0.02;
            break;
        case 'z': case 'Z':
            this->scale.z += f*0.02;
            break;
        default:
            // TODO: throw exception
            break;
    }
}

Mesh Mesh::load(string model_name) {
    vector<float> vArr;
    vector<int> tArr;
    int nt = 0, nv = 0, r;
    char line_header[128];
    
    FILE* file = fopen(model_name.c_str(), "r");
    if (file == NULL) {
        // TODO: throw exception
    }
    
    while ((r = fscanf(file, "%s", line_header)) != EOF) {
        if (strcmp(line_header, "v") == 0) {
            float x, y, z;
            fscanf(file, "%f %f %f\n", &x, &y, &z);
            vArr.push_back(x);
            vArr.push_back(y);
            vArr.push_back(z);
            nv++;
        } else if (strcmp(line_header, "vt") == 0) {
            // ignore for now
        } else if (strcmp(line_header, "vn") == 0) {
            // ignore for now (computed when creatig the mesh)
        } else if (strcmp(line_header, "f") == 0) {
            int v1, v2, v3;
            fscanf(file, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d\n",
                    &v1, &v2, &v3);
            tArr.push_back(v1-1);
            tArr.push_back(v2-1);
            tArr.push_back(v3-1);
            nt++;
        } else {
            // ignore for now
        }
    }
    return Mesh(nv, nt, &vArr[0], &tArr[0]);
}
