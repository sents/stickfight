
#OBJS specifies which files to compile as part of the project
OBJS = test.cc utility.cc lTexture.o physobj.o bezier.o stickman.o
#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -std=c++11

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = stickfight

#This is the target that compiles our executable
stickman.o: stickman.cc bezier.o physobj.o
	$(CC) stickman.cc $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o stickman.o
bezier.o: bezier.cc
	$(CC) bezier.cc $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o bezier.o
physobj.o: physobj.cc
	$(CC) physobj.cc $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o physobj.o
lTexture.o: lTexture.cc
	$(CC) lTexture.cc $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o lTexture.o
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
