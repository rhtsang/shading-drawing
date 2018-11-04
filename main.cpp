#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "polygon.h"
#include "filemanipulation.h"
#include "pixel.h"
#include "line.h"
#include "rasterize.h"
#include "transform.h"
#include "clip.h"

float *PixelBuffer;
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

    // transformation prompt
	int polygonID;
    char transformation;
    char lineMode;
	cout << "Enter the ID of the polygon you want to manipulate (indexing starts at 0): ";
	cin >> polygonID;
    cout << "Enter the first letter of the transformation you want performed. 's' for scale, 'r' for rotate, 't' for translate: ";
    cin >> transformation;
    cout << "How would you like edges to be drawn? Enter 'b' for Bresenham or 'd' for DDA:";
    cin >> lineMode;

    vector<Polygon> polygons;
	readFile(argv[1], polygons);
    if (transformation == 't') {
        float translate_x, translate_y;
        cout << "How far along the x-axis would you like to translate? ";
        cin >> translate_x;
        cout << "How far along the x-axis would you like to translate? ";
        cin >> translate_y;
        translate(polygons, polygonID, translate_x, translate_y);
    } else if (transformation == 'r') {
        float angle;
        cout << "How far would you like to rotate? (enter an angle in degrees) ";
        cin >> angle;
        rotate(polygons, polygonID, angle);
    } else if (transformation == 's') {
        float factor;
        cout << "How much would you like to scale? ";
        cin >> factor;
        scale(polygons, polygonID, factor);
    }
    writeFile(argv[2], polygons);

    Coordinate viewport(width, height);

    // clip window
    float x_min, x_max, y_min, y_max;
    cout << "Enter x_min of the clip window: ";
    cin >> x_min;
    cout << "Enter x_max of the clip window: ";
    cin >> x_max;
    cout << "Enter y_min of the clip window: ";
    cin >> y_min;
    cout << "Enter y_max of the clip window: ";
    cin >> y_max;

    // get clipped vertices before rasterizing and drawing to screen
    vector<Polygon> clipped = clip(polygons, x_min, x_max, y_min, y_max);
	rasterize(PixelBuffer, clipped, viewport, lineMode);


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(width/2, height/2);

	//create and set main window title
	int MainWindow = glutCreateWindow("ECS175 Project 1");
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
