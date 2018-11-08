#include "rasterize.h"
#include <vector>
#include "polygon.h"
#include <algorithm>
#include <math.h>
#include "line.h"
#include "pixel.h"
#include <iostream>

using namespace std;

// based on the solution described on https://piazza.com/class/jmz4kfexw6n6rc?cid=24
// and endorsed on https://piazza.com/class/jmz4kfexw6n6rc?cid=51
void rasterize(float* PixelBuffer, vector<Polygon> polygons, Coordinate viewport) {

    for (vector<Polygon>::iterator itr = polygons.begin(); itr != polygons.end(); itr++) {

        // intermediate buffer to draw single polygon at a time
        float* DrawBuffer = new float[int(viewport.x) * int(viewport.y) * 3];

        for (vector<Triangle>::iterator itr2 = (itr->triangleFaces).begin(); itr2 != (itr->triangleFaces).end(); itr2++) {
            dda(DrawBuffer, (itr->vertices).at(itr2->v1), (itr->vertices).at(itr2->v2), viewport);
            dda(DrawBuffer, (itr->vertices).at(itr2->v2), (itr->vertices).at(itr2->v3), viewport);
            dda(DrawBuffer, (itr->vertices).at(itr2->v3), (itr->vertices).at(itr2->v1), viewport);
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
            for (int x = x_start; x <= x_end; x++) {
                int left_index = (y * viewport.x + x_start) * 3;
                int right_index = (y * viewport.x + x_end) * 3;
                float left = DrawBuffer[left_index];
                float right = DrawBuffer[right_index];
cout << "Left intensity: " << left << " | Right intensity: " << right << endl;
                if (x_end != x_start) {
                    double intensity = (((float)x_end-x)/((float)x_end-x_start))*(left) + (((float)x-x_start)/((float)x_end-x_start))*(right);
//cout << "Intensity at point (" << x << ", " << y << "): " << intensity << endl;
                    Coordinate point(x,y,0);
                    setPixel(DrawBuffer, point, viewport, intensity);
                }
            }
        }

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
}
