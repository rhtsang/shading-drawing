#include "filemanipulation.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "polygon.h"
#include <string>

using namespace std;

void readPolygons(char* fileName, vector<Polygon>& polygons) {

    ifstream inputFile(fileName);
    string line;
    int numPolygons;

    // General parsing algorithm taken from
    // https://stackoverflow.com/questions/39970420/c-parsing-data-from-file-i-o
    // and modified to fit this assignment

    // Get the number of polygons from the first line, then skip to description
    // of polygons
    inputFile >> numPolygons;
    getline(inputFile, line);
    getline(inputFile, line);

    // construct a Polygon object, one by one
    for (int i = 0; i < numPolygons; i++) {

        // Get current polygon's number of vertices
        int numVertices;
        inputFile >> numVertices;
        getline(inputFile, line);

        // Get coordinates of the current polygon's vertices
        vector<Coordinate> coordinates;
        for (int j = 0; j < numVertices; j++) {

            float float_x, float_y, float_z;
            string string_x, string_y, string_z;
            getline(inputFile, line);
            istringstream coordinate(line);
            getline(coordinate, string_x, ' ');
            getline(coordinate, string_y, ' ');
            getline(coordinate, string_z);
            float_x = stof(string_x);
            float_y = stof(string_y);
            float_z = stof(string_z);

            // Store current polygon's vertices info in a vector
            Coordinate point(float_x, float_y, float_z);
            coordinates.push_back(point);

        }

        // Get edges of current polygon
        int numTriangleFaces;
        inputFile >> numTriangleFaces;
        getline(inputFile, line);

        vector<Triangle> triangleFaces;
        for (int j = 0; j < numTriangleFaces; j++) {
            int int_v1, int_v2, int_v3;
            string string_v1, string_v2, string_v3;
            getline(inputFile, line);
            istringstream face(line);
            getline(face, string_v1, ' ');
            getline(face, string_v2, ' ');
            getline(face, string_v3);
            int_v1 = stoi(string_v1);
            int_v2 = stoi(string_v2);
            int_v3 = stoi(string_v3);

            // Store current polygon's vertices info in a vector
            Triangle triangle(int_v1, int_v2, int_v3);
            triangleFaces.push_back(triangle);

        }

        // Store current polygon's info in a vector
        Polygon polygon(numVertices, coordinates, numTriangleFaces, triangleFaces);
        polygons.push_back(polygon);

        // go to next Polygon
        getline(inputFile, line);
    }

}

void writePolygons(char* fileName, vector<Polygon>& polygons) {

    ofstream outputFile(fileName);

    outputFile << polygons.size() << "\n\n";

    // write each Polygon to output file
    for (vector<Polygon>::iterator itr = polygons.begin(); itr != polygons.end(); itr++) {

        // write vertices info
        outputFile << itr->numVertices << endl;
        for (vector<Coordinate>::iterator itr2 = (itr->vertices).begin(); itr2 != (itr->vertices).end(); itr2++) {
            outputFile << itr2->x << ' ' << itr2->y << ' ' << itr2->z << endl;
        }

        // write edges int_from
        outputFile << itr->numTriangleFaces << endl;
        for (vector<Triangle>::iterator itr2 = (itr->triangleFaces).begin(); itr2 != (itr->triangleFaces).end(); itr2++) {
            outputFile << itr2->v1 << ' ' << itr2->v2 << ' ' << itr2->v3 << endl;
        }
    }

}
