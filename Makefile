CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include

BINDIR=bin/
BIN=$(BINDIR)/main
SRC=src/
INCLUDES=./include/
SRCS=$(shell find $(SRC) -name '*.cpp')

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES) $(BINDIR)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS) -I$(INCLUDES)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR)
