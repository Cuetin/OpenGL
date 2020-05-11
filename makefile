main: main.o shader.o
	g++ main.o shader.o -o main -lglfw -lglew -framework OpenGL
main.o: main.cpp
	g++ -c main.cpp
shader.o: shaders/shader.cpp shaders/shader.hpp
	g++ -c shaders/shader.cpp