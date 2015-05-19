CC := g++
CFLAGS := -O3 -g -Wall -Werror -Wno-unknown-pragmas -std=c++11 -D_USE_MATH_DEFINES #-Wextra
INCLUDES := -I/usr/include/GL/
LDFLAGS := -lm -lGL -lGLEW -lglut 
SRCS := Main.cpp Ray.cpp Sphere.cpp  
OBJS := $(SRCS:.cpp=.o)
MAIN := raytracer

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *~ $(MAIN)
