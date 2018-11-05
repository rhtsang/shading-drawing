#ifndef FILEMANIPULATION_H
#define FILEMANIPULATION_H

#include <vector>
#include "polygon.h"

using namespace std;

void readPolygons(char* fileName, vector<Polygon>& polygons);
void writePolygons(char* fileName, vector<Polygon>& polygons);


#endif
