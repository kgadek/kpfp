CXX = gcc
TARGET = kpfp
SOURCE = kpfp.c
OBJECTS = $(SOURCE:%.c=%.o)
CFLAGS = -O3
LDFLAGS = -lGL -lGLU -lglut -lm

all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

$(OBJECTS): $(SOURCE)
	$(CXX) -c $(CFLAGS) $(SOURCE)

clean:
	rm -f $(OBJECTS)
	rm -f *~
	rm -f core
