#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "MiscUtil.h"
#include "Map.h"
#include "Timer.h"
#include "IterativeDeepening.h"
#include "Alphabeta.h"

/**
 * Recherche par approfondissement itératif.
 * Effectue une recherche en profondeur, limitée au niveau 2, puis 4, etc, jusqu'à l'obtention d'une solution.
 * Cela permet de simuler une recherche en largeur, avec une complexité O(1) en espace mémoire.
 *
 * @param map Plateau
 * @return Mouvent effectué par le P1
 */
int iterativeDeepening(const Map& map) {
	searchCount = 0;

	int x = map.MyX();
	int y = map.MyY();
	
#ifdef DEBUG
	log << "************************Making move***************************\n";
	log.flush();
#endif

	Pos p;
	p.x = x;
	p.y = y;
	
	Pos p2;
	p2.x = map.OpponentX();
	p2.y = map.OpponentY();
	
	int maxDepth = 4;
	bool done = false;
	int bestDir = 1;
	while (!done) {
#ifdef TIMER
		log << "depth=" << maxDepth << "\n";
		log.flush();
#endif
		int newBestDir = alphabeta(map.board, p, p2, 0, 0, INT_MIN, INT_MAX, maxDepth);
		
		if (timedOut == true) {
			done = true;
		} else {
			bestDir = newBestDir;
			if (maxDepth >= 50 * 50) {
				done = true;
			} else {
				maxDepth += 2;
			}
		}
	}

#ifdef TIMER
		log << "searchCount=" << searchCount << "\n";
		log.flush();
#endif
	return bestDir;
}

