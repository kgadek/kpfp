CC 		= g++
CXXFLAGS 	= -O2 
LDFLAGS 	= -lglut -lGL -lGLU\
		-lGLEW\
		-lXext -lXmu -lX11 -lm

PROJECTS = simple_shader

.phony: all clean $(PROJECTS)

all: $(PROJECTS)

simple_shader: simple_shader_p

clean:
	rm simple_shader_p