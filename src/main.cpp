// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127


#include "utils.hpp"

WindowManager windowManager = WindowManager(800, 800, "Hierarchical Modelling");

int main(int argc, char** argv) {
    windowManager.initBuffersGL();
    windowManager.mainLoop();
    return 0;
}
