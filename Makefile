
#OBJS specifies which files to compile as part of the project
OBJS = main.cc utility.cc lTexture.o physobj.o

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = stickfight

#This is the target that compiles our executable
physobj.o: physobj.cc
	$(CC) physobj.cc $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o physobj.o
lTexture.o: lTexture.cc
	$(CC) lTexture.cc $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o lTexture.o
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
