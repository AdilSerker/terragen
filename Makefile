PLATFORM = $(shell uname)

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	LFLAGS = -L/usr/local/lib -lglfw -lGLEW -lGL
	EXT = 
endif

INC = include
MODULE = src/modules/*.cpp
WFLAGS = -Wall 
CFLAGS = -O3 -ffast-math

all: src/main.cpp 
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) $< $(MODULE) -I $(INC) -c

app: src/App.cpp
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) $< -c

scene: src/Scene.cpp
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) $< -c

hm: src/Heightmap.cpp
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) $< -c

camera: src/CameraOrbit.cpp
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) $< -c

light: src/LightDirectional.cpp
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) $< -c

build: 
	rm -f terragen && g++ ./*.o $(LFLAGS) -I $(INC) -o terragen

clean:
	rm -f terragen *.o

