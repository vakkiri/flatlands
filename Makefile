OBJS = *.cpp */*.cpp */*/*.cpp

CC = g++

COMPILER_FLAGS = -I . -std=c++17 -Wall -Wextra -Werror -Wpedantic

DEBUG = -g

LINKER_FLAGS = -lSDL2 -lSDL2_mixer -lSDL2_net -DGL_GLEXT_PROTOTYPES -lIL -lILU

OBJ_NAME = flatlands

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LINKER_FLAGS += -lGL
    DEBUG += -pg
endif
ifeq ($(UNAME_S),Darwin)
    LINKER_FLAGS += -framework OpenGL
endif

all: $(OBJS)
	$(CC) $(COMPILER_FLAGS) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)

debug: $(OBJS)
	$(CC) $(COMPILER_FLAGS) $(OBJS) $(LINKER_FLAGS) $(DEBUG) -o $(OBJ_NAME)

