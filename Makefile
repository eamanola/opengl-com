OBJS = glad/src/gl.c stb_image/src/stb_image.c \
	*.cpp \
	skeletal/*.cpp \
	characters/*.cpp \
	playground/*.cpp playground/skybox/*.cpp \
	disco/*.cpp \
	gl-utils/*.cpp \
	shaders/*.cpp \
	shaders/deferred/*.cpp \
	utils/*.cpp \
	shadow-maps/*.cpp

CC = g++

COMPILER_FLAGS = -Wall -Werror -g

LINKER_FLAGS = -lglfw -lassimp# -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

OBJ_NAME = output

INCLUDES = -Iglad/include \
-Istb_image/include \
-I.

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDES) -o $(OBJ_NAME)
