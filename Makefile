OBJS = glad/src/gl.c stb_image/src/stb_image.c \
*.cpp #main.cpp util.cpp shape.cpp shader.cpp

CC = g++

COMPILER_FLAGS = -Wall -Werror #-g

LINKER_FLAGS = -lglfw# -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

OBJ_NAME = output

INCLUDES = -Iglad/include -Istb_image/include

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDES) -o $(OBJ_NAME)
