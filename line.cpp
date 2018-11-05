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
