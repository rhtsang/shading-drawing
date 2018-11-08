#ifndef POLYGON_H
#define  POLYGON_H

#include <vector>

using namespace std;

class Coordinate {
public:

    float x;
    float y;
    float z;
    double intensity;

    Coordinate(float x1, float y1, float z1);

};

class Triangle {
public:

    int v1;
    int v2;
    int v3;

    Triangle(int a, int b, int c);

};

class Polygon {
public:

    int numVertices;
    vector<Coordinate> vertices;
    int numTriangleFaces;
    vector<Triangle> triangleFaces;

    Polygon(int numVerts, vector<Coordinate> verts, int numTris, vector<Triangle> triFaces);

};

#endif
