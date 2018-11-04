all: draw

clean:
	rm -f draw filemanipulation.o polygon.o transform.o projection.o

draw: main.cpp filemanipulation.o polygon.o transform.o projection.o
	g++ main.cpp -lglut -lGL -o draw filemanipulation.o polygon.o transform.o projection.o

filemanipulation.o: filemanipulation.cpp filemanipulation.h
	g++ -std=c++11 -c filemanipulation.cpp

polygon.o: polygon.cpp polygon.h
	g++ -c polygon.cpp

transform.o: transform.cpp transform.h
	g++ -c transform.cpp

projection.o: projection.cpp projection.h
	g++ -c projection.cpp
