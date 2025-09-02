CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
CPPFLAGS=-std=c++23 -g
LDFLAGS=-L/usr/local/lib

BINDIR=bin
BIN=$(BINDIR)/main
MODELS=./models
SRC=src
INCLUDEDIR=./include
SRCS=$(shell find $(SRC) -name '*.cpp')
OBJS=$(patsubst $(SRC)/%.cpp,$(BINDIR)/%.o,$(SRCS))
INCLUDES=$(shell find $(INCLUDEDIR) -name '*.hpp')

all: $(BIN) $(MODELS)

$(MODELS):
	mkdir -p $(MODELS)

$(BIN): $(OBJS) $(BINDIR)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(BIN) $(LDFLAGS) $(LIBS) -I$(INCLUDEDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/%.o: $(SRC)/%.cpp $(INCLUDES) | $(BINDIR)
	$(CC) -c $< -o $@ $(CPPFLAGS) -I$(INCLUDEDIR)

clean:
	rm -rf $(BINDIR)
