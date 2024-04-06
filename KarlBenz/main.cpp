#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <vector>
#include "neat.h"
#include "population.h"
#include "driver.h"
using namespace std;

int main() {
    NEAT::Population* p = 0;

    srand((unsigned)time(NULL));
    NEAT::load_neat_params("parameters.ne", true);
    p = Driver(100);

    if (p)
        delete p;

    return(0);
}
