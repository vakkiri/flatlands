OBJS = *.cpp */*.cpp */*/*.cpp

CC = g++

COMPILER_FLAGS = -std=c++17 -Wall -Wextra -Werror -Wpedantic

DEBUG = -g -pg

LINKER_FLAGS = -lSDL2 -lSDL2_mixer -lSDL2_net -lGL -DGL_GLEXT_PROTOTYPES -lIL -lILU

OBJ_NAME = flatlands

all: $(OBJS)
	$(CC) $(COMPILER_FLAGS) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)

debug: $(OBJS)
	$(CC) $(OBJS) $(LINKER_FLAGS) $(DEBUG) -o $(OBJ_NAME)

