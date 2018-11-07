#ifndef RASTERIZE_H
#define RASTERIZE_H

#include <vector>
#include "polygon.h"

using namespace std;

void rasterize(float* PixelBuffer, vector<Polygon> polygons, Coordinate viewport);

#endif
