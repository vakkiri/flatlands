OBJS = *.cpp */*.cpp

CC = g++

COMPILER_FLAGS = -std=c++13 -Wall -g

LINKER_FLAGS = -lSDL2 -lGL -DGL_GLEXT_PROTOTYPES -lIL -lILU

OBJ_NAME = flatlands

all: $(OBJS)
	$(CC) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)

