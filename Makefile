OBJS = glad/src/gl.c main.cpp util.cpp triangle.cpp

CC = g++

COMPILER_FLAGS = -Wall -Werror

LINKER_FLAGS = -lglfw# -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

OBJ_NAME = output

INCLUDES = -Iglad/include

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDES) -o $(OBJ_NAME)
