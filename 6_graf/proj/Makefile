LDFLAGS = -lGL -lGLU -lglut -lGLEW
all:
	g++ -o example.out obj.cpp light_example.cpp -lGL -lGLU -lglut -lGLEW
purge:
	rm example.out

light_example: obj.cpp

%: %.cpp
	g++ $< $(LDFLAGS) -o $@

