#include "rasterize.h"
#include <vector>
#include "polygon.h"
#include <algorithm>
#include <math.h>
#include "line.h"
#include "pixel.h"

using namespace std;

// based on the solution described on https://piazza.com/class/jmz4kfexw6n6rc?cid=24
// and endorsed on https://piazza.com/class/jmz4kfexw6n6rc?cid=51
void rasterize(float* PixelBuffer, vector<Polygon> polygons, Coordinate viewport, char lineMode) {

    for (vector<Polygon>::iterator itr = polygons.begin(); itr != polygons.end(); itr++) {

        // intermediate buffer to draw single polygon at a time
        float* DrawBuffer = new float[int(viewport.x) * int(viewport.y) * 3];

        // draw all edges for current polygon
		for (int i = 0; i < (itr->vertices).size(); i++) {
			if (i == (itr->vertices).size() - 1) {
                if (lineMode == 'd') {
    				dda(DrawBuffer, (itr->vertices).at(i), (itr->vertices).at(0), viewport);
                } else if (lineMode == 'b') {
    				bresenham(DrawBuffer, (itr->vertices).at(i), (itr->vertices).at(0), viewport);
                }
			} else {
                if (lineMode == 'd') {
    	        	dda(DrawBuffer, (itr->vertices).at(i), (itr->vertices).at(i+1), viewport);
                } else if (lineMode == 'b') {
    				bresenham(DrawBuffer, (itr->vertices).at(i), (itr->vertices).at(i+1), viewport);
                }
			}
	    }

        // fill in current polygon
        for (int y = 0; y < viewport.y; y++) {
            int x_start, x_end;

            // find start intersection
            for (x_start = 0; x_start < viewport.x; x_start++) {
                int index = (y * viewport.x + x_start) * 3;
                if (DrawBuffer[index] == 1) {
                    break;
                }
            }

            // find end intersection
            for (x_end = viewport.x - 1; x_end >= 0; x_end--) {
                int index = (y * viewport.x + x_end) * 3;
                if (DrawBuffer[index] == 1) {
                    break;
                }
            }

            // set pixels in between intersections
            for (int x = x_start; x <= x_end; x++) {
                int index = (y * viewport.x + x) * 3;
                Coordinate point(x,y);
                setPixel(DrawBuffer, point, viewport);
            }
        }

        // copy DrawBuffer to PixelBuffer
        for (int y = 0; y < viewport.y; y++) {
            for (int x = 0; x < viewport.x; x++) {
                int index = (y * viewport.x + x) * 3;
                if (DrawBuffer[index] == 1) {
                    Coordinate point(x,y);
                    setPixel(PixelBuffer, point, viewport);
                }
            }
        }

	}
}
