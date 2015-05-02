#include <cstdio>
#include <cstring>
#include <cmath>
#include <limits>
#include "Mesh.h"

int Mesh::bounding_type = Mesh::SPHERE_BOUNDING;

Mesh::Mesh(vector<Vector> vertices, vector<Vector> vnorms, vector<Triangle> triangles, bool is_bounding) {
    this->nv = vertices.size();
    this->nt = triangles.size();
    this->vertices.resize(this->nv);
    this->vnorms.resize(this->nv);
    this->triangles.resize(this->nt);
    this->vertices = vertices;
    this->vnorms = vnorms;
    this->triangles = triangles;
    this->scale = Vector(1, 1, 1);
    this->rotation = Vector(0, 0, 0);
    this->translation = Vector(0, 0, 0);
  
    this->is_bounding = is_bounding;
    if (!is_bounding) {
        this->ComputeBoundingSphere();
    }
}

Mesh::Mesh(int nv, int nt, float *vArr, int *tArr, bool is_bounding) : nv(nv), nt(nt) {
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
            this->vnorms[t.vInds[j]] += ab.Cross(ac).Normalized();
        }
    }
    for (int i = 0; i < nv; i++) {
        this->vnorms[i] = this->vnorms[i].Normalized();
    }
  
    this->is_bounding = is_bounding;
    if (!is_bounding) {
        this->ComputeBoundingSphere();
    }
}

void Mesh::BoundingType(int bounding_type) {
    bounding_type = bounding_type;
}

int Mesh::BoundingType() {
    return bounding_type;
}
int Mesh::NumVertices() {
    return this->nv;
}

int Mesh::NumTriangles() {
    return this->nt;
}

bool Mesh::IsBounding() {
    return this->is_bounding;
}

Matrix Mesh::TransformationMatrix() {
    return Matrix::Translation(translation)
            * Matrix::Rotation('x', rotation.x)
            * Matrix::Rotation('y', rotation.y)
            * Matrix::Rotation('z', rotation.z)
            * Matrix::Scale(scale);
}

void Mesh::Move(char dir) {
    this->Move(dir, 1.0);
}

void Mesh::Rotate(char dir) {
    this->Rotate(dir, 1.0);
}

void Mesh::Scale(char dir) {
    this->Scale(dir, 1.0);
}

void Mesh::Move(Vector v) {
    this->Move('x', v.x);
    this->Move('y', v.y);
    this->Move('z', v.z);
}

void Mesh::Rotate(Vector v) {
    this->Rotate('x', v.x);
    this->Rotate('y', v.y);
    this->Rotate('z', v.z);
}

void Mesh::Scale(Vector v) {
    this->Scale('x', v.x);
    this->Scale('y', v.y);
    this->Scale('z', v.z);
}

void Mesh::Move(char dir, float units) {
    float f = -units;
    if (dir < 'A' || dir > 'Z') {
        f = units;
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
    if (!this->is_bounding) {
        this->bounding_volume->Move(dir, units);
    }
}

void Mesh::Rotate(char dir, float units) {
    float f = units;
    if (dir < 'A' || dir > 'Z') {
        f = -units;
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
    if (!this->is_bounding) {
        this->bounding_volume->Rotate(dir, units);
    }
}

void Mesh::Scale(char dir, float units) {
    float f = units;
    if (dir < 'A' || dir > 'Z') {
        f = -units;
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
    if (!this->is_bounding) {
        this->bounding_volume->Scale(dir, units);
    }
}

Mesh Mesh::Load(string model_name, bool is_bounding) {
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
    
    fclose(file);
    
    return Mesh(nv, nt, &vArr[0], &tArr[0], is_bounding);
}

float d(Vector a, Vector b) {
    return (a - b).Length();
}

void Mesh::ComputeBoundingSphere() {
    Vector xmax = Vector(std::numeric_limits<float>::min(), 0, 0);
    Vector xmin = Vector(std::numeric_limits<float>::max(), 0, 0);
    Vector ymax = Vector(0, std::numeric_limits<float>::min(), 0);
    Vector ymin = Vector(0, std::numeric_limits<float>::max(), 0);
    Vector zmax = Vector(0, 0, std::numeric_limits<float>::min());
    Vector zmin = Vector(0, 0, std::numeric_limits<float>::max());
    for (unsigned int i = 0; i < this->vertices.size(); i++) {
        if (this->vertices[i].x > xmax.x) xmax = this->vertices[i];
        if (this->vertices[i].y > ymax.y) ymax = this->vertices[i];
        if (this->vertices[i].z > zmax.z) zmax = this->vertices[i];
        if (this->vertices[i].x < xmin.x) xmin = this->vertices[i];
        if (this->vertices[i].y < ymin.y) ymin = this->vertices[i];
        if (this->vertices[i].z < zmin.z) zmin = this->vertices[i];
    }
    Vector p1 = xmin, p2 = xmax;
    if (d(p1, p2) < d(ymin, ymax)) {
        p1 = ymin;
        p2 = ymax;
    }
    if (d(p1, p2) < d(zmin, zmax)) {
        p1 = zmin;
        p2 = zmax;
    }
    Vector center = (p1 + p2).ScalarMult(0.5);
    float rad = d(p1, p2)/2;
    for (unsigned int i = 0; i < this->vertices.size(); i++) {
        float dist = d(center, this->vertices[i]);
        if (dist > rad) {
            rad = (rad + dist)/2;
            center = (center.ScalarMult(rad) + this->vertices[i].ScalarMult(dist-rad)).ScalarMult(1/dist);
        }
    }
    Mesh tmp = Mesh::Load("models/sphere.obj", true);
    this->bounding_sphere.radius = rad;
    this->bounding_sphere.center = center;
    this->bounding_volume = new Mesh(tmp.vertices, tmp.vnorms, tmp.triangles, true);
    this->bounding_volume->translation = Vector(center.x, center.y, center.z);
    this->bounding_volume->scale = Vector(rad, rad, rad);
}

float Mesh::BoundingSphereRadius() {
    return this->bounding_sphere.radius;
}

Vector Mesh::BoundingSphereCenter() {
    return this->bounding_sphere.center;
}
