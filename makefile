all: main.o front.o
	g++ -o front main.o front.o	-framework GLUT -framework OpenGL
        
main.o: main.cpp
	 g++ -c main.cpp -I ./
 
front.o: front.cpp
	 g++ -std=c++11 -pthread -c front.cpp -I ./
 
clean:
	 rm -rf *.o
	 rm -rf front
              