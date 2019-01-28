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

app: src/modules/App.cpp
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) -I $(INC) $< -c

scene: src/modules/Scene.cpp
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) -I $(INC) $< -c

hm: src/modules/Heightmap.cpp
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) -I $(INC) $< -c

camera: src/modules/CameraOrbit.cpp
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) -I $(INC) $< -c

light: src/modules/LightDirectional.cpp
	g++ -std=gnu++11 $(CFLAGS) $(WFLAGS) -I $(INC) $< -c

build: 
	rm -f terragen && g++ ./*.o $(LFLAGS) -I $(INC) -o terragen

clean:
	rm -f terragen *.o

