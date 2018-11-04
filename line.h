#include "line.h"
#include "pixel.h"
#include <math.h>
#include "polygon.h"
#include <algorithm>

using namespace std;

// DDA line drawing algorithm, pretty much copy-pasted from textbook page 134
void dda(float* PixelBuffer, Coordinate start, Coordinate end, Coordinate viewport) {

    int dx = end.x - start.x,
        dy = end.y - start.y,
        steps,
        k;
    float xIncrement, yIncrement, x = start.x, y = start.y;

    if (fabs(dx) > fabs(dy)) {
        steps = fabs(dx);
    } else {
        steps = fabs(dy);
    }

    xIncrement = float(dx) / float(steps);
    yIncrement = float(dy) / float(steps);

    Coordinate roundedStart(round(start.x), round(start.y));
    setPixel(PixelBuffer, roundedStart, viewport);

    for (k = 0; k < steps; k++) {
        x += xIncrement;
        y += yIncrement;
        Coordinate point(round(x), round(y));
        setPixel(PixelBuffer, point, viewport);
    }

}

// general Bresenham algorithm taken from the textbook page 137
// modified to fit cases of slope == 1, slope == -1
void bresenham(float* PixelBuffer, Coordinate start, Coordinate end, Coordinate viewport) {

    if ((end.y - start.y) == (end.x - start.x)) { // slope == 1
        int y = min(round(start.y), round(end.y));
        for (int x = min(round(start.x), round(end.x)); x <= max(round(start.x), round(end.x)); x++) {
            Coordinate point(x, y++);
            setPixel(PixelBuffer, point, viewport);
        }
        return;
    } else if ((end.y - start.y) == -1 * (end.x - start.x)) { // slope == -1
        int y;
        for (int x = min(round(start.x), round(end.x)), y = max(round(start.y), round(end.y)); x <= max(round(start.x), round(end.x)); x++, y--) {
            Coordinate point(x, y);
            setPixel(PixelBuffer, point, viewport);
        }
        return;
    }

    // the rest of this is based on the code given in the textbook page 137
    float slope = (end.y - start.y) / (end.x - start.x);
    if (fabs(slope) < 1) {
        int dx = fabs(end.x - start.x),
            dy = fabs(end.y - start.y),
            p = 2 * dy - dx,
            twoDy = 2 * dy,
            twoDyMinusDx = 2 * (dy - dx),
            x, y;

        if (start.x > end.x) {
            x = end.x;
            y = end.y;
            end.x = start.x;
        } else {
            x = start.x;
            y = start.y;
        }

        Coordinate roundedStart(round(x), round(y));
        setPixel(PixelBuffer, roundedStart, viewport);


        while (x < end.x) {
            x++;
            if (p < 0) {
                p += twoDy;
            } else {
                if (slope > 0) {y++;} else {y--;}
                p += twoDyMinusDx;
            }
            Coordinate point(round(x), round(y));
            setPixel(PixelBuffer, point, viewport);
        }
    } else if (fabs(slope) > 1) {
        int dx = fabs(end.x - start.x),
            dy = fabs(end.y - start.y),
            p = 2 * dx - dy,
            twoDy = 2 * dx,
            twoDyMinusDx = 2 * (dx - dy),
            x, y;

        if (start.y > end.y) {
            x = end.x;
            y = end.y;
            end.y = start.y;
        } else {
            x = start.x;
            y = start.y;
        }

        Coordinate roundedStart(round(x), round(y));
        setPixel(PixelBuffer, roundedStart, viewport);


        while (y < end.y) {
            y++;
            if (p < 0) {
                p += twoDy;
            } else {
                if (slope > 0) {
                    x++;
                } else {
                    x--;
                    //cout << "wtf" << endl;
                }
                p += twoDyMinusDx;
            }
            Coordinate point(round(x), round(y));
            setPixel(PixelBuffer, point, viewport);
        }
    }
}
