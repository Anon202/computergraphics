CC := g++
CFLAGS := -O3 -g -Wall -Werror -std=c++11 -D_USE_MATH_DEFINE
INCLUDES := -I/usr/include/GL/
LDFLAGS := -lm -lGL -lglut -lGLU -pthread
SRCS := Main.cpp Ray.cpp Sphere.cpp Image.cpp Scene.cpp SimpleRayTracer.cpp Light.cpp Plane.cpp ../common/Camera.cpp ../algebra/Matrix.cpp ../algebra/Vector.cpp ../algebra/HomVector.cpp EasyBMP/EasyBMP.cpp
OBJS := $(SRCS:.cpp=.o)
MAIN := raytracer

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *~ $(MAIN) ../algebra/*.o ../common/*.o
