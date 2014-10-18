#include <stdio.h>
#include <stdlib.h>
#include "MiscUtil.h"
#include "Map.h"
#include <string.h>

/**
 * Score pour une égalité.
 */
int SCORE_DRAW = 0;

// 4 tableaux de taille max (w*h) afin de ne pas avoir à les réallouer
// 2 pour chaque joueur. 1er : liste des positions à explorer (étape n), 2eme = liste des positions à explorer (étape n + 1)
Pos *q1;
Pos *q1new;
Pos *q2;
Pos *q2new;

unsigned char *voronoiBoard;

/**
 * Effectue un déplacement de joueur pour le calcul de la partition de Voronoi.
 *
 * @param voronoiBoard Plateau temporaire
 * @param q Mouvements à effectuer (modifié par effet de bord pour le coup suivant)
 * @param qnew Nouveaux mouvements (temp)
 * @param qCount Nombre de mouvements à effectuer (modifié par effet de bord, mis à jour pour le coup suivant)
 * @param moveCount Nombre de mouvements totaux effectivement parcourus (modifié par effet de bord)
 * @param playerIndex Indice du joueur (0, 1)
 */
int voronoiMove(unsigned char *voronoiBoard, Pos *q, Pos *qnew, int *qCount, int *moveCount, int playerIndex) {
	int qCountTmp;
	Pos *currentPos;
	Pos newPos;
	int i, j;
	int px, py;
	
	// Effectue les mouvements du joueur
	qCountTmp = 0;
	for (i = 0; i < *qCount; i++) {
		currentPos = &q[i];
#ifdef DEBUG
//	log << "Testing player " << playerIndex << " pos x = " << currentPos->x << " y = " << currentPos->y << "\n";
//	log.flush();
#endif
		// Vérifie si la position n'a pas déjà été comsommée
		// ** DEBUG optim inline **
		px = currentPos->x;
		py = currentPos->y;
		bool isWall = true;
		if (px < 0 || py < 0 || px >= map->map_width || py >= map->map_height) {
			// NOP
		} else {
			unsigned char c = voronoiBoard[(py << 6) + px];
			if (c == ' ') {
				isWall = false;
			}
		}
		// ** FIN optim inline **

//		if (!isWall(voronoiBoard, currentPos->x, currentPos->y, false, false)) {
		if (!isWall) {
			// Parcours cette case
			voronoiBoard[(py << 6) + px] = playerIndex == 0 ? '1' : '2';	
#ifdef DEBUG
//	dumpBoard(voronoiBoard);	
#endif

		/*	// ** DEBUG optim inline 1 **
			AvailableMove availableMove;
			availableMove.count = 0;
	
			for (j = 1; j <= 4; j++) {
				Pos p2 = *currentPos;
				p2.x += relativePosX[j];
				p2.y += relativePosY[j];

				// ** DEBUG optim inline 2 **
				bool isWall2 = true;
				if (p2.x < 0 || p2.y < 0 || p2.x >= map->map_width || p2.y >= map->map_height) {
					// NOP
				} else {
					unsigned char c = voronoiBoard[(p2.y << 6) + p2.x];
					if (c == ' ') {
						isWall2 = false;
					}
				}
				// ** FIN optim inline 2 **

				if (!isWall2) {
					availableMove.move[availableMove.count] = j;
					availableMove.count++;
				}
			}
			// ** DEBUG optim inline 1 **
*/

			AvailableMove availableMove = getAvailableMove(voronoiBoard, *currentPos);
			for (j = 0; j < availableMove.count; j++) {
				// Calcule la position relative
				Pos p2 = *currentPos;
				p2.x += relativePosX[availableMove.move[j]];
				p2.y += relativePosY[availableMove.move[j]];
				
				qnew[qCountTmp] = p2;
				qCountTmp++;
			}
			(*moveCount)++;
		}
	}
#ifdef DEBUG
//	log << "Found = " << qCountTmp << " new moves, total = " << (*moveCount) << "\n";
//	log.flush();
#endif

	// Initialise le nouveau nombre de mouvements à jouer
	*qCount = qCountTmp;
}

/**
 * Calcule le score associé au "territoire de Voronoi" d'un plateau.
 *
 * @param board Plateau
 * @param p1 Position initiale du joueur 1
 * @param p2 Position initiale du joueur 2
 * @param playerIndex Indice du joueur du point de vue duquel on évalue le score (0, 1)
 */
int voronoiScore(unsigned char *board, Pos p1, Pos p2, int playerIndex) {
	// Test si on est collisionné
	if (p1.x == p2.x && p1.y == p2.y) {
		return SCORE_DRAW;
	}

	copyBoard(board, voronoiBoard);
	
	int i, j;

	// Ajoute les positions initiales du joueur 1
	AvailableMove a1init = getAvailableMove(voronoiBoard, p1);
	for (i = 0; i < a1init.count; i++) {
		q1[i] = p1;
		q1[i].x += relativePosX[a1init.move[i]];
		q1[i].y += relativePosY[a1init.move[i]];
	}
	int q1Count = a1init.count;
	
	// Ajoute les positions initiales du joueur 
	AvailableMove a2init = getAvailableMove(voronoiBoard, p2);
	for (i = 0; i < a2init.count; i++) {
		q2[i] = p2;
		q2[i].x += relativePosX[a2init.move[i]];
		q2[i].y += relativePosY[a2init.move[i]];
	}
	int q2Count = a2init.count;

	// Nombre total de positions accessibles par les joueurs 1 et 2
	int move1Count = 0;
	int move2Count = 0;
	
	Pos *qtmp;
	do {
		if (playerIndex == 0) {
			voronoiMove(voronoiBoard, q1, q1new, &q1Count, &move1Count, 0);
			qtmp = q1;
			q1 = q1new;
			q1new = qtmp;
	
			voronoiMove(voronoiBoard, q2, q2new, &q2Count, &move2Count, 1);
			qtmp = q2;
			q2 = q2new;
			q2new = qtmp;
		} else {
			voronoiMove(voronoiBoard, q2, q2new, &q2Count, &move2Count, 1);
			qtmp = q2;
			q2 = q2new;
			q2new = qtmp;

			voronoiMove(voronoiBoard, q1, q1new, &q1Count, &move1Count, 0);
			qtmp = q1;
			q1 = q1new;
			q1new = qtmp;
		}
#ifdef DEBUG
//	log << "Coups restants q1Count = " << q1Count << " q2Count = " << q2Count << "\n";
//	log.flush();
#endif
	} while (q1Count > 0 || q2Count > 0);

#ifdef DEBUG
//	log << "diff = " << move1Count << " - " << move2Count << "\n";
//	log.flush();
#endif

	if (playerIndex == 0) {
		return move2Count - move1Count;
	} else {
		return move1Count - move2Count;
	}
}

/**
 * A appeler une fois en début de coup pour allouer les variables temporaires de Voronoi.
 */
void initVoronoi() {
	q1 = (Pos *) malloc(map->map_height * 64 * sizeof(Pos));
	q1new = (Pos *) malloc(map->map_height * 64 * sizeof(Pos));
	q2 = (Pos *) malloc(map->map_height * 64 * sizeof(Pos));
	q2new = (Pos *) malloc(map->map_height * 64 * sizeof(Pos));

	voronoiBoard = (unsigned char *) malloc(map->map_height * 64 * sizeof(unsigned char));
}


/**
 * A appeler en fin de coup pour libérer les variables temporaires de Voronoi.
 */
void freeVoronoi() {
	free(q1);
	free(q1new);
	free(q2);
	free(q2new);
	free(voronoiBoard);
}
