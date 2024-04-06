#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include "neat.h"
#include "network.h"
#include "population.h"
#include "organism.h"
#include "genome.h"
#include "species.h"

using namespace std;

using namespace NEAT;

Population *Driver(int gens);
bool Driver_Evaluation(Organism *org);
int Driver_Epoch(Population *pop,int generation,char *filename, int &winnernum, int &winnergenes,int &winnernodes);

#endif






