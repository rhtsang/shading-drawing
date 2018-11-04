#include "filemanipulation.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "polygon.h"
#include <string>

using namespace std;

void readFile(char* fileName, vector<Polygon>& polygons) {

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
        int numEdges;
        inputFile >> numEdges;
        getline(inputFile, line);

        vector<Edge> edges;
        for (int j = 0; j < numEdges; j++) {
            int int_from, int_to;
            string string_from, string_to;
            getline(inputFile, line);
            istringstream coordinate(line);
            getline(coordinate, string_from, ' ');
            getline(coordinate, string_to);
            int_from = stoi(string_from);
            int_to = stoi(string_to);

            // Store current polygon's vertices info in a vector
            Edge edge(int_from, int_to);
            edges.push_back(edge);
        }

        // Store current polygon's info in a vector
        Polygon polygon(numVertices, coordinates, numEdges, edges);
        polygons.push_back(polygon);

        // go to next Polygon
        getline(inputFile, line);
    }

}

void writeFile(char* fileName, vector<Polygon>& polygons) {

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
        outputFile << itr->numEdges << endl;
        for (vector<Edge>::iterator itr2 = (itr->edges).begin(); itr2 != (itr->edges).end(); itr2++) {
            outputFile << itr2->from << ' ' << itr2->to << endl;
        }
    }

}
