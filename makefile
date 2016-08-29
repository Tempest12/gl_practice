#Compiler and flags
#Author Shane del Solar
CC     = g++
CFLAGS = -Wall -g -std=c++11 
LINKER = -lglfw -lX11 -ldl -lXi -lXinerama -lXcursor -lXrandr -lXxf86vm -lGLEW -lGLU -lGL -lm -pthread

GLOBJECT = Main.o Vector3f.o Camera.o Quaternion.o PulseCounterf.o MathUtil.o

EXE = gl

all: test 

test: $(GLOBJECT)
	$(CC) $(CFLAGS) $(GLOBJECT) $(LINKER) -o $(EXE)

clean:
	rm -f *.o $(EXE)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<