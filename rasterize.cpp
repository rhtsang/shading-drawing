#include "rasterize.h"
#include <vector>
#include "polygon.h"
#include <algorithm>
#include <math.h>
#include "line.h"
#include "pixel.h"
#include <iostream>
#include <cstdio>

using namespace std;

class Face {
public:
    Coordinate v1;
    Coordinate v2;
    Coordinate v3;

    Face(Coordinate a, Coordinate b, Coordinate c) {
        v1.x = a.x; v1.y = a.y; v1.z = a.z; v1.intensity = a.intensity;
        v2.x = b.x; v2.y = b.y; v2.z = b.z; v2.intensity = b.intensity;
        v3.x = c.x; v3.y = c.y; v3.z = c.z; v3.intensity = c.intensity;
    }
};

//https://www.geeksforgeeks.org/bubble-sort/

void swap(Face* xp, Face* yp)
{
    Face temp = *xp;
    *xp = *yp;
    *yp = temp;
}

float minVertex(Face face, int dimension) {
    if (dimension == 1) { // min of x coordinate
        return min(min(face.v1.x, face.v2.x), face.v3.x);
    } else if (dimension == 2) { // min of y coordinate
        return min(min(face.v1.y, face.v2.y), face.v3.y);
    } else if (dimension == 0) { // min of z coordinate
        return min(min(face.v1.z, face.v2.z), face.v3.z);
    }
}

double averageIntensity(float* DrawBuffer, int x, int y, Coordinate viewport) {
    float intensity = 0;

    if (x-1 >= 0) {
        if (y-1 >= 0) {
            int index = ((y-1) * viewport.x + (x-1)) * 3;
            intensity += DrawBuffer[index];
        }
        int index = ((y) * viewport.x + (x-1)) * 3;
        intensity += DrawBuffer[index];
        if (y+1 < viewport.y) {
            int index = ((y+1) * viewport.x + (x-1)) * 3;
            intensity += DrawBuffer[index];
        }
    }

    if (y-1 >= 0) {
        int index = ((y-1) * viewport.x + (x)) * 3;
        intensity += DrawBuffer[index];
    }
    int index = ((y) * viewport.x + (x)) * 3;
    intensity += DrawBuffer[index];
    if (y+1 < viewport.y) {
        int index = ((y+1) * viewport.x + (x)) * 3;
        intensity += DrawBuffer[index];
    }

    if (x+1 < viewport.x) {
        if (y-1 >= 0) {
            int index = ((y-1) * viewport.x + (x+1)) * 3;
            intensity += DrawBuffer[index];
        }
        int index = ((y) * viewport.x + (x+1)) * 3;
        intensity += DrawBuffer[index];
        if (y+1 < viewport.y) {
            int index = ((y+1) * viewport.x + (x+1)) * 3;
            intensity += DrawBuffer[index];
        }
    }
    intensity /= 9.0f;
    return intensity;
}

void bubbleSort(vector<Face>& faces, int dimension)
{
   int i, j;
   int n = faces.size();
   bool swapped;
   for (i = 0; i < n-1; i++)
   {
     swapped = false;
     for (j = 0; j < n-i-1; j++)
     {
        if (minVertex(faces.at(j), dimension) < minVertex(faces.at(j+1), dimension))
        {
           swap(&(faces.at(j)), &(faces.at(j+1)));
           swapped = true;
        }
     }

     // IF no two elements were swapped by inner loop, then break
     if (swapped == false)
        break;
   }
}

// based on the solution described on https://piazza.com/class/jmz4kfexw6n6rc?cid=24
// and endorsed on https://piazza.com/class/jmz4kfexw6n6rc?cid=51
void rasterize(float* PixelBuffer, vector<Polygon> polygons, Coordinate viewport, int dimension, bool halftone) {

    vector<Face> faces;
    for (vector<Polygon>::iterator itr = polygons.begin(); itr != polygons.end(); itr++) {
        for (vector<Triangle>::iterator itr2 = (itr->triangleFaces).begin(); itr2 != (itr->triangleFaces).end(); itr2++) {
            Coordinate c1 = (itr->vertices).at(itr2->v1);
            Coordinate c2 = (itr->vertices).at(itr2->v2);
            Coordinate c3 = (itr->vertices).at(itr2->v3);
            //Face face((itr->vertices).at(itr2->v1), (itr->vertices).at(itr2->v2), (itr->vertices).at(itr2->v3));
//cout << "Intensities: " << c1.intensity << ' ' << c2.intensity << ' ' << c3.intensity << endl;
            Face face(c1, c2, c3);
            faces.push_back(face);
        }
    }
    bubbleSort(faces, dimension);
    for (vector<Face>::iterator itr = faces.begin(); itr != faces.end(); itr++) {
//cout << "Coordinate 1: " << itr->v1.x << ' ' << itr->v1.y << ' ' << itr->v1.z << endl;
//cout << "Coordinate 2: " << itr->v2.x << ' ' << itr->v2.y << ' ' << itr->v2.z << endl;
//cout << "Coordinate 3: " << itr->v3.x << ' ' << itr->v3.y << ' ' << itr->v3.z << endl;
    }

    for (vector<Face>::iterator itr = faces.begin(); itr != faces.end(); itr++) {

        // intermediate buffer to draw single polygon at a time
        float* DrawBuffer = new float[int(viewport.x) * int(viewport.y) * 3];
        if (dimension == 0) { // xy plane
            Coordinate c1(itr->v1.x, itr->v1.y, 0);
            Coordinate c2(itr->v2.x, itr->v2.y, 0);
            Coordinate c3(itr->v3.x, itr->v3.y, 0);
            c1.intensity = itr->v1.intensity;
            c2.intensity = itr->v2.intensity;
            c3.intensity = itr->v3.intensity;
//cout << "Intensities: " << c1.intensity << ' ' << c2.intensity << ' ' << c3.intensity << endl;
            dda(DrawBuffer, c1, c2, viewport);
            dda(DrawBuffer, c2, c3, viewport);
            dda(DrawBuffer, c3, c1, viewport);
        } else if (dimension == 1) { // yz plane
//cout << "pre seg fault?" << endl;
//fflush(stdout);
            Coordinate c1(itr->v1.y, itr->v1.z, 0);
            Coordinate c2(itr->v2.y, itr->v2.z, 0);
            Coordinate c3(itr->v3.y, itr->v3.z, 0);
            c1.intensity = itr->v1.intensity;
            c2.intensity = itr->v2.intensity;
            c3.intensity = itr->v3.intensity;
            dda(DrawBuffer, c1, c2, viewport);
            dda(DrawBuffer, c2, c3, viewport);
            dda(DrawBuffer, c3, c1, viewport);
//cout << "post seg fault?" << endl;
//fflush(stdout);
        } else if (dimension == 2) { // xz plane
            Coordinate c1(itr->v1.x, itr->v1.z, 0);
            Coordinate c2(itr->v2.x, itr->v2.z, 0);
            Coordinate c3(itr->v3.x, itr->v3.z, 0);
            c1.intensity = itr->v1.intensity;
            c2.intensity = itr->v2.intensity;
            c3.intensity = itr->v3.intensity;
            dda(DrawBuffer, c1, c2, viewport);
            dda(DrawBuffer, c2, c3, viewport);
            dda(DrawBuffer, c3, c1, viewport);
        }
        // fill in current polygon
        for (int y = 0; y < viewport.y; y++) {
            int x_start, x_end;

            // find start intersection
            for (x_start = 0; x_start < viewport.x; x_start++) {
                int index = (y * viewport.x + x_start) * 3;
                if (DrawBuffer[index] > 0) {
                    break;
                }
            }

            // find end intersection
            for (x_end = viewport.x - 1; x_end >= 0; x_end--) {
                int index = (y * viewport.x + x_end) * 3;
                if (DrawBuffer[index] > 0) {
                    break;
                }
            }

            // set pixels in between intersections
            // normal shading, i.e. not half-toning
            for (int x = x_start; x <= x_end; x++) {
                int left_index = (y * viewport.x + x_start) * 3;
                int right_index = (y * viewport.x + x_end) * 3;
                float left = DrawBuffer[left_index];
                float right = DrawBuffer[right_index];
//cout << "Left intensity: " << left << " | Right intensity: " << right << endl;
                if (x_end != x_start) {
                    double intensity = (((float)x_end-x)/((float)x_end-x_start))*(left) + (((float)x-x_start)/((float)x_end-x_start))*(right);
//cout << "Intensity at point (" << x << ", " << y << "): " << intensity << endl;
                    Coordinate point(x,y,0);
                    setPixel(DrawBuffer, point, viewport, intensity);
                }
            }

//             // transform normal shading to half toning
//             for (int y = 0; y < ((int) viewport.y/3)*3; y += 3) {
//                 for (int x = 0; x < ((int) viewport.x/3)*3; x += 3) {
//                     float intensity = averageIntensity(DrawBuffer, x, y, viewport);
// if (intensity > 0)
// cout << "Point (" << x << ", " << y << ") intensity: " << intensity << endl;
//                 }
//             }
//         }

        // copy DrawBuffer to PixelBuffer
        for (int y = 0; y < viewport.y; y++) {
            for (int x = 0; x < viewport.x; x++) {
                int index = (y * viewport.x + x) * 3;
                if (DrawBuffer[index] > 0) {
                    Coordinate point(x,y,0);
                    setPixel(PixelBuffer, point, viewport, DrawBuffer[index]);
//cout << "Transfer DrawBuffer to PixelBuffer value: " << DrawBuffer[index] << endl;
                }
            }
        }
	}
    if (halftone) {
        // transform normal shading to half toning
        for (int y = 0; y < ((int) viewport.y/3)*3; y += 3) {
            for (int x = 0; x < ((int) viewport.x/3)*3; x += 3) {
                float intensity = averageIntensity(PixelBuffer, x, y, viewport);
    // if (intensity > 0)
    //cout << "Point (" << x << ", " << y << ") intensity: " << intensity << endl;
                for (int i = 0; i < floor(intensity*10); i++) {
    //cout << floor(intensity*10) << endl;
                    int index = rand() % 10;
    //cout << "index: " << index << endl;
    //fflush(stdout);
                    int bufferIndex = 0;
                    switch (index) {
                        case 1: if (x-1 >= 0 && y+1 < viewport.y) bufferIndex = ((y+1) * viewport.x + (x-1)) * 3; break;
                        case 2: if (y+1 < viewport.y) bufferIndex = ((y+1) * viewport.x + (x)) * 3; break;
                        case 3: if (x+1 < viewport.x && y+1 < viewport.y) bufferIndex = ((y+1) * viewport.x + (x+1)) * 3; break;
                        case 4: if (x-1 >= 0) bufferIndex = ((y) * viewport.x + (x-1)) * 3; break;
                        case 5: bufferIndex = ((y) * viewport.x + (x)) * 3; break;
                        case 6: if (x+1 < viewport.x) bufferIndex = ((y) * viewport.x + (x+1)) * 3; break;
                        case 7: if (x-1 >= 0 && y-1 >= 0) bufferIndex = ((y-1) * viewport.x + (x-1)) * 3; break;
                        case 8: if (y-1 >= 0) bufferIndex = ((y-1) * viewport.x + (x)) * 3; break;
                        case 9: if (x+1 < viewport.x && y-1 >= 0) bufferIndex = ((y-1) * viewport.x + (x+1)) * 3; break;
                        default: break;
                    }
                    //PixelBuffer[bufferIndex] = PixelBuffer[bufferIndex+1] = PixelBuffer[bufferIndex+2] = 1;
    //cout << "bufferIndex: " << bufferIndex << endl;
    //fflush(stdout);
                    if (PixelBuffer[bufferIndex] == 1.0f) i=i;
                    else PixelBuffer[bufferIndex] = PixelBuffer[bufferIndex+1] = PixelBuffer[bufferIndex+2] = 1;
                }
            }
        }

        for (int y = 0; y < viewport.y; y++) {
            for (int x = 0; x < viewport.x; x++) {
                int index = (y * viewport.x + x) * 3;
                if (PixelBuffer[index] != 1)
                    PixelBuffer[index] = PixelBuffer[index+1] = PixelBuffer[index+2] = 0;
    //cout << "Clearing pixel (" << x << ", " << y << ")" << endl;
            }
        }
    }
}

}
