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
vector<Polygon> polygons;
int width, height;

void display();

int main(int argc, char *argv[])
{

	// User interaction

    // viewport
    cout << "Enter the width of the viewing window: ";
    cin >> width;
    cout << "Enter the height of the viewing window: ";
    cin >> height;
	PixelBuffer = new float[width * height * 3];

    // read in polygon data from user input file
	readPolygons(argv[1], polygons);

    int phongConstant;
    double ambient, diffuse, specular, ambientIntensity, sourceIntensity;
    float lightSourceX, lightSourceY, lightSourceZ, fromX, fromY, fromZ;

    // read in environment data from user input file
    readEnvironment(argv[2], phongConstant, ambient, diffuse, specular, ambientIntensity, sourceIntensity, lightSourceX, lightSourceY, lightSourceZ, fromX, fromY, fromZ);

    Coordinate lightSource(lightSourceX, lightSourceY, lightSourceZ);
    Coordinate viewPoint(fromX, fromY, fromZ);
    double k = averageDistanceFromLightSource(polygons, lightSource);
//cout << "Average distance: " << k << endl;

    // calculate intensities at polygon vertices
    for (vector<Polygon>::iterator itr = polygons.begin(); itr != polygons.end(); itr++) {
        vector<double> intensities = phongIntensity(*itr, 4, 0.5, 0.25, 0.75, 5, 9,
                                lightSource, viewPoint, k);
    }


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

    bool halftone;
    cout << "Would you like to enable half tone? Enter 0 for no or 1 for yes: ";
    cin >> halftone;

    int projection;
    cout << "Which plane would you like to project to? Enter 0 for xy plane, 1 for yz plane, or 2 for xz plane: ";
    cin >> projection;

    Coordinate viewport(width, height, 0);
    rasterize(PixelBuffer, polygons, viewport, projection, halftone);

	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, PixelBuffer);

    // clear the screen of pixels to prepare for next display
    for (int y = 0; y < viewport.y; y++) {
        for (int x = 0; x < viewport.x; x++) {
            int index = (y * viewport.x + x) * 3;
            PixelBuffer[index] = PixelBuffer[index+1] = PixelBuffer[index+2] = 0;
        }
    }

	//window refresh

	glFlush();
    glutPostRedisplay();
}
