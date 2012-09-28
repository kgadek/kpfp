LDFLAGS = -lGL -lGLU -lglut -lGLEW
all:
	g++ -o example.out obj.cpp light_example.cpp -lGL -lGLU -lglut -lGLEW
purge:
	rm example.out
%: %.cpp
	g++ $< $(LDFLAGS) -o $@
