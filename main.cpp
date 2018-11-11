#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "polygon.h"
#include "filemanipulation.h"
#include "pixel.h"
#include "line.h"
#include "rasterize.h"
#include "phong.h"
#include "rasterize.h"

float *PixelBuffer;
int width, height;
void display();

int main(int argc, char *argv[])
{

	// User interaction

    // viewport
    // cout << "Enter the width of the viewing window: ";
    // cin >> width;
    // cout << "Enter the height of the viewing window: ";
    // cin >> height;
    width = height = 200;
	PixelBuffer = new float[width * height * 3];
    Coordinate viewport(width, height, 0);
    // transformation prompt

    vector<Polygon> polygons;
	readPolygons(argv[1], polygons);

    int phongConstant;
    double ambient, diffuse, specular, ambientIntensity, sourceIntensity;
    float lightSourceX, lightSourceY, lightSourceZ, fromX, fromY, fromZ;

    readEnvironment(argv[2], phongConstant, ambient, diffuse, specular, ambientIntensity, sourceIntensity, lightSourceX, lightSourceY, lightSourceZ, fromX, fromY, fromZ);

    Coordinate lightSource(lightSourceX, lightSourceY, lightSourceZ);
    Coordinate viewPoint(fromX, fromY, fromZ);
    double k = averageDistanceFromLightSource(polygons, lightSource);
//cout << "Average distance: " << k << endl;
    for (vector<Polygon>::iterator itr = polygons.begin(); itr != polygons.end(); itr++) {
        vector<double> intensities = phongIntensity(*itr, 4, 0.5, 0.25, 0.75, 5, 9,
                                lightSource, viewPoint, k);
    }
    rasterize(PixelBuffer, polygons, viewport, 1);

    //writePolygons(argv[2], polygons);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(width/2, height/2);

	//create and set main window title
	int MainWindow = glutCreateWindow("ECS175 Project 3");
	glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
	glutDisplayFunc(display);

	glutMainLoop();//main display loop, will display until terminate

	return 0;
}

//main display loop, this function will be called again and again by OpenGL
void display()
{
	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, PixelBuffer);

	//window refresh
	glFlush();
}
