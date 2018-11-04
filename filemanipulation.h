#ifndef FILEMANIPULATION_H
#define FILEMANIPULATION_H

#include <vector>
#include "polygon.h"

using namespace std;

void readFile(char* fileName, vector<Polygon>& polygons);
void writeFile(char* fileName, vector<Polygon>& polygons);


#endif
