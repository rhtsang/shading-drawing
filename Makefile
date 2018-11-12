all: draw

clean:
	rm -f draw filemanipulation.o polygon.o phong.o line.o pixel.o rasterize.o

draw: main.cpp filemanipulation.o polygon.o phong.o line.o pixel.o rasterize.o
	g++ main.cpp -lglut -lGL -o draw filemanipulation.o polygon.o phong.o line.o pixel.o rasterize.o

filemanipulation.o: filemanipulation.cpp filemanipulation.h
	g++ -std=c++11 -c filemanipulation.cpp

polygon.o: polygon.cpp polygon.h
	g++ -c polygon.cpp

phong.o: phong.cpp phong.h
	g++ -c phong.cpp

pixel.o: pixel.cpp pixel.h
	g++ -c pixel.cpp

line.o: line.cpp line.h
	g++ -c line.cpp

rasterize.o: rasterize.cpp rasterize.h
	g++ -c rasterize.cpp
