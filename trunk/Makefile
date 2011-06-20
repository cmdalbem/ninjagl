CC = g++

FLAGS = -Wall -std=c++0x

OBJDIR = obj
OBJNAMES = Object.o Material.o vector3f.o vector4f.o
OBJS = $(addprefix $(OBJDIR)/,$(OBJNAMES))

LIBS = -lGL -lglut -lGLU -lglui -ljpeg

BIN = main

all: main.cpp $(OBJS)
	$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS) 

$(OBJDIR)/%.o: %.cpp %.h | $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 

$(OBJDIR):
	mkdir $(OBJDIR) -p


.PHONY: clean
clean:
	rm -f $(BIN)
	rm -f *.o
	rm -f $(OBJDIR)
