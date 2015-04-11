CC := g++
CFLAGS := -O3 -g -Wall -Werror -Wno-unknown-pragmas
INCLUDES := -I/usr/include/GL/
LDFLAGS := -lm -lGL -lGLEW -lglut 
SRCS := main.cpp mesh.cpp algebra.cpp 
OBJS := $(SRCS:.cpp=.o)
MAIN := assignment1

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *~ $(MAIN)
