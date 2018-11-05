all: draw

clean:
	rm -f draw filemanipulation.o polygon.o projection.o phong.o

draw: main.cpp filemanipulation.o polygon.o phong.o
	g++ main.cpp -lglut -lGL -o draw filemanipulation.o polygon.o phong.o

filemanipulation.o: filemanipulation.cpp filemanipulation.h
	g++ -std=c++11 -c filemanipulation.cpp

polygon.o: polygon.cpp polygon.h
	g++ -c polygon.cpp

phong.o: phong.cpp phong.h
	g++ -c phong.cpp
