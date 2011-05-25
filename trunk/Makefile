CC = g++

FLAGS = -Wall -std=c++0x

OBJS = Object.o Material.o vector3f.o vector4f.o
OBJLIB =

LIBS = -lGL -lglut -lGLU -lglui

BIN = main

%.o: %.cpp %.h
	$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 


all: main.cpp $(OBJS) $(OBJLIB)
	$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS) 


clean:
	rm -f $(BIN)
	rm -f *.o
