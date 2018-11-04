#include "polygon.h"

using namespace std;

void setPixel(float* PixelBuffer, Coordinate point, Coordinate viewport) {
    int index = (point.y * viewport.x + point.x) * 3;
    PixelBuffer[index] = PixelBuffer[index+1] = PixelBuffer[index+2] = 1;
}
