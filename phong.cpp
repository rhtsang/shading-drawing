#include "phong.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;

Coordinate findCentroid(Polygon polygon) {
    double centroid_x = 0;
    double centroid_y = 0;
    double centroid_z = 0;
    for (vector<Coordinate>::iterator itr = polygon.vertices.begin(); itr != polygon.vertices.end(); itr++) {
        centroid_x += itr->x;
        centroid_y += itr->y;
        centroid_z += itr->z;
    }
    centroid_x /= polygon.numVertices;
    centroid_y /= polygon.numVertices;
    centroid_z /= polygon.numVertices;

    Coordinate centroid(centroid_x, centroid_y, centroid_z);
    return centroid;
}

double averageDistanceFromLightSource(vector<Polygon> polygons, Coordinate lightSource) {
    double totalDistance = 0;
    for (vector<Polygon>::iterator itr = polygons.begin(); itr != polygons.end(); itr++) {
        Coordinate centroid = findCentroid(*itr);
        double polygonDistance = sqrt(pow(lightSource.x-centroid.x, 2) + pow(lightSource.y-centroid.y, 2) + pow(lightSource.z-centroid.z, 2));
        totalDistance += polygonDistance;
    }
    totalDistance /= polygons.size();
    return totalDistance;
}

vector<double> phongIntensity(Polygon polygon, int phongConstant, double ambient, double diffuse, double specular, double ambientIntensity, double sourceIntensity, Coordinate lightSource, Coordinate from, double averageDistance) {

    vector<double> intensities;
    vector<Coordinate> vertices = polygon.vertices;

    for (int i = 0; i < vertices.size(); i++) {
        // calculate view vector
        double viewVectorX = from.x - vertices.at(i).x;
        double viewVectorY = from.y - vertices.at(i).y;
        double viewVectorZ = from.z - vertices.at(i).z;
        double viewVectorMagnitude = sqrt(pow(viewVectorX,2) + pow(viewVectorY,2) + pow(viewVectorZ,2));
        viewVectorX /= viewVectorMagnitude;
        viewVectorY /= viewVectorMagnitude;
        viewVectorZ /= viewVectorMagnitude;
//cout << "View vector " << viewVectorX << ' ' << viewVectorY << ' ' << viewVectorZ << endl;
        // calc light vector
        double lightVectorX = lightSource.x - vertices.at(i).x;
        double lightVectorY = lightSource.y - vertices.at(i).y;
        double lightVectorZ = lightSource.z - vertices.at(i).z;
        double lightVectorMagnitude = sqrt(pow(lightVectorX,2) + pow(lightVectorY,2) + pow(lightVectorZ,2));
        lightVectorX /= lightVectorMagnitude;
        lightVectorY /= lightVectorMagnitude;
        lightVectorZ /= lightVectorMagnitude;
//cout << "Light vector" << lightVectorX << ' ' << lightVectorY << ' ' << lightVectorZ << endl;
        // calc normal vector
        Coordinate* p1 = NULL;
        Coordinate* p2 = NULL;
        double sumNormalX = 0;
        double sumNormalY = 0;
        double sumNormalZ = 0;
        for (vector<int>::iterator itr = vertices.at(i).adjacentVertices.begin(); itr != vertices.at(i).adjacentVertices.end(); itr++) {
            p1 = &(vertices.at(*itr));
            for (vector<int>::iterator itr2 = (p1->adjacentVertices).begin(); itr2 != (p1->adjacentVertices).end(); itr2++) {
                for (vector<int>::iterator itr3 = vertices.at(*itr2).adjacentVertices.begin(); itr3 != vertices.at(*itr2).adjacentVertices.end(); itr3++) {
                    if (i == *itr3) {
                        p2 = &(vertices.at(*itr2));
                        goto label;
                        //break;
                    }
                }
                //break;
            }
label:
//cout << "Current point: " << vertices.at(i).x << ' ' << vertices.at(i).y << ' ' << vertices.at(i).z << endl;
//cout << "First point: " << p1->x << ' ' << p1->y << ' ' << p1->z << endl;
//cout << "Second point: " << p2->x << ' ' << p2->y << ' ' << p2->z << endl;
            Coordinate d1(p1->x - vertices.at(i).x, p1->y - vertices.at(i).y, p1->z - vertices.at(i).z);
            Coordinate d2(p2->x - vertices.at(i).x, p2->y - vertices.at(i).y, p2->z - vertices.at(i).z);
            double normalX = ((d1.y)*(d2.z)) - ((d1.z)*(d2.y));
            double normalY = -1 * ((d1.x)*(d2.z)) - ((d1.z)*(d2.x));
            double normalZ = ((d1.x)*(d2.y)) - ((d1.y)*(d2.x));
//cout << "Normal vector: " << normalX << ' ' << normalY << ' ' << normalZ << endl;
            sumNormalX += normalX;
            sumNormalY += normalY;
            sumNormalZ += normalZ;
//cout << "after label: " << sumNormalX << ' ' << sumNormalY << ' ' << sumNormalZ << endl;
        }
        sumNormalX /= vertices.at(i).adjacentVertices.size() - 1;
        sumNormalY /= vertices.at(i).adjacentVertices.size() - 1;
        sumNormalZ /= vertices.at(i).adjacentVertices.size() - 1;
        double denominator = sqrt(pow(sumNormalX,2) + pow(sumNormalY,2) + pow(sumNormalZ,2));
//cout << "Denominator " << denominator << endl;
        sumNormalX /= denominator;
        sumNormalY /= denominator;
        sumNormalZ /= denominator;
//cout << "Normal vector " << ' ' << sumNormalX << ' ' << sumNormalY << ' ' << sumNormalZ << endl;
        // calc reflection vector
        double reflectX = (-1*lightVectorX) + 2*(sumNormalX*lightVectorX + sumNormalY*lightVectorY + sumNormalZ*lightVectorZ)*sumNormalX;
        double reflectY = (-1*lightVectorY) + 2*(sumNormalX*lightVectorX + sumNormalY*lightVectorY + sumNormalZ*lightVectorZ)*sumNormalY;
        double reflectZ = (-1*lightVectorZ) + 2*(sumNormalX*lightVectorX + sumNormalY*lightVectorY + sumNormalZ*lightVectorZ)*sumNormalZ;

        double kI = ambient*ambientIntensity;
        double fractionThing = sourceIntensity/(viewVectorMagnitude + averageDistance);
        double LdotN = (lightVectorX*sumNormalX) + (lightVectorY*sumNormalY) + (lightVectorZ*sumNormalZ);
        double RdotV = (reflectX*viewVectorX) + (reflectY*viewVectorY) + (reflectZ*viewVectorZ);

        double smallParentheses1 = diffuse * LdotN;
        double smallParentheses2 = specular * pow(RdotV, phongConstant);

        double intensity = kI + (fractionThing*(smallParentheses1 + smallParentheses2));
        intensities.push_back(intensity);
    }

    // normalize intensities to [0,1] range
    // find min, max, delta
    double min = intensities.at(0);
    double max = intensities.at(0);
    for (vector<double>::iterator itr = intensities.begin(); itr != intensities.end(); itr++) {
        if (*itr < min) min = *itr;
        if (*itr > max) max = *itr;
    }
    double delta = max-min;

    for (vector<double>::iterator itr = intensities.begin(); itr != intensities.end(); itr++) {
        *itr = (*itr-min)/delta;
        cout << "Intensity: " << *itr << endl;
    }

    return intensities;

}
