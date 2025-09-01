CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 

BINDIR=bin
BIN=$(BINDIR)/main
SRC=src
INCLUDEDIR=./include
SRCS=$(shell find $(SRC) -name '*.cpp')
OBJS=$(patsubst $(SRC)/%.cpp,$(BINDIR)/%.o,$(SRCS))
INCLUDES=$(shell find $(INCLUDEDIR) -name '*.hpp')

all: $(BIN)

$(BIN): $(OBJS) $(BINDIR)
	g++ $(CPPFLAGS) $(OBJS) -o $(BIN) $(LDFLAGS) $(LIBS) -I$(INCLUDEDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/%.o: $(SRC)/%.cpp $(INCLUDES) | $(BINDIR)
	$(CC) -c $< -o $@ $(CPPFLAGS) -I$(INCLUDEDIR)

clean:
	rm -rf $(BINDIR)
