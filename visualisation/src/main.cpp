#include "Visualisation.h"


int main() {
    using namespace F9Sim::Graphics;

    if(!Visualisation_init())
        return 1;

    Visualisation_run();

    return 0;
}
