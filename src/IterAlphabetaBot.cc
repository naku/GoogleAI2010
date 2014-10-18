#include "Map.h"
#include "MiscUtil.h"
#include "Voronoi.h"
#include "IterativeDeepening.h"
#include "Timer.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

Map *map;

int main() {
#ifdef DEBUG
	log.open("debug.log");
	log << "Starting Bot\n";
	log.flush();
#endif
#ifdef TIMER
	log.open("debug.log");
	log << "Starting Bot\n";
	log.flush();
#endif
	while (true) {
		Map newMap;
		map = &newMap;

		initVoronoi();
		startTimer();
		Map::MakeMove(iterativeDeepening(*map));
		endTimer();
		freeVoronoi();
	}
#ifdef DEBUG
	log.close();
#endif
#ifdef TIMER
	log.close();
#endif
	return 0;
}
