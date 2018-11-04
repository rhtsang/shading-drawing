#include "projection.h"
#include <GL/glut.h>
#include <cstdlib>

vector<Polygon> bound(vector<Polygon> polygons) {

    vector<Polygon> boundedPolygons;

    float x_min = polygons.at(0).vertices.at(0).x;
    float x_max = polygons.at(0).vertices.at(0).x;
    float y_min = polygons.at(0).vertices.at(0).y;
    float y_max = polygons.at(0).vertices.at(0).y;
    float z_min = polygons.at(0).vertices.at(0).z;
    float z_max = polygons.at(0).vertices.at(0).z;

    //loop thru to find extrema
    for (vector<Polygon>::iterator itr = polygons.begin(); itr != polygons.end(); itr++) {
        for (vector<Coordinate>::iterator itr2 = (itr->vertices).begin(); itr2 != (itr->vertices).end(); itr2++) {
            if (itr2->x < x_min) x_min = itr2->x;
            if (itr2->x > x_max) x_max = itr2->x;
            if (itr2->y < y_min) y_min = itr2->y;
            if (itr2->y > y_max) y_max = itr2->y;
            if (itr2->z < z_min) z_min = itr2->z;
            if (itr2->z > z_max) z_max = itr2->z;
        }
    }

    // add margins to sides
    x_min--;
    x_max++;
    y_min--;
    y_max++;
    z_min--;
    z_max++;

    float delta_x = x_max - x_min;
    float delta_y = y_max - y_min;
    float delta_z = z_max - z_min;
    float delta = max( max(delta_x, delta_y), delta_z);

    //loop thru to map world coordinates to normalized device coordinates
    for (vector<Polygon>::iterator itr = polygons.begin(); itr != polygons.end(); itr++) {
        vector<Coordinate> NDC;
        for (vector<Coordinate>::iterator itr2 = (itr->vertices).begin(); itr2 != (itr->vertices).end(); itr2++) {
            float x_ndc = (itr2->x - x_min) / delta;
            float y_ndc = (itr2->y - y_min) / delta;
            float z_ndc = (itr2->z - z_min) / delta;
            Coordinate point(x_ndc, y_ndc, z_ndc);
            NDC.push_back(point);
        }
        Polygon poly(itr->numVertices, NDC, itr->numEdges, itr->edges);
        boundedPolygons.push_back(poly);
    }

    return boundedPolygons;

}

void project(vector<Polygon> polygons) {
    glBegin(GL_LINES);
    for (vector<Polygon>::iterator itr = polygons.begin(); itr != polygons.end(); itr++) {
        vector<Coordinate> vertices = itr->vertices;
        for (vector<Edge>::iterator itr2 = (itr->edges).begin(); itr2 != (itr->edges).end(); itr2++) {
            float r = ((float) rand())/ ((float) RAND_MAX);
            float g = ((float) rand())/ ((float) RAND_MAX);
            float b = ((float) rand())/ ((float) RAND_MAX);

            // map world coordinates to NDC coordinates in 3 different quadrants on display window
            // upper left: XY plane
            // upper right: YZ plane
            // lower right: XZ plane
            glColor3f(r, g, b);
            // XY plane in upper left quadrant
            glVertex2f(vertices.at(itr2->from - 1).x - 1, vertices.at(itr2->from - 1).y);
            glVertex2f(vertices.at(itr2->to - 1).x - 1, vertices.at(itr2->to - 1).y);
            // YZ plane in upper right quadrant
            glVertex2f(vertices.at(itr2->from - 1).y, vertices.at(itr2->from - 1).z);
            glVertex2f(vertices.at(itr2->to - 1).y, vertices.at(itr2->to - 1).z);
            // XZ plane in lower right quadrant
            glVertex2f(vertices.at(itr2->from - 1).x, vertices.at(itr2->from - 1).z - 1);
            glVertex2f(vertices.at(itr2->to - 1).x, vertices.at(itr2->to - 1).z - 1);
        }
    }
    glEnd();
}
