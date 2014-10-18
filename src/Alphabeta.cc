#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "MiscUtil.h"
#include "Map.h"
#include "Alphabeta.h"
#include "Voronoi.h"
#include "Timer.h"

int alphaBetaCount;

/**
 * Evalue un coup avec l'algorithme alpha-beta.
 *
 * @param board Plateau
 * @param p1 Position initiale du joueur 1
 * @param p2 Position initiale du joueur 2
 * @param playerIndex Indice du joueur en cours (0 ou 1)
 * @param depth Profondeur de l'évaluation en cours (racine = 0)
 * @param alpha Alpha (meilleur score pour le joueur 0)
 * @param beta Beta (meilleur score pour le joueur 1)
 * @param maxDepth Profondeur max. (multiple de 2).
 * @return Meilleur score dans le sous-arbre
 */
int alphabeta(unsigned char *board, Pos p1, Pos p2, int playerIndex, int depth, int alpha, int beta, int maxDepth) {
	// Si timeout, retourne le plus vite (une valeur quelconque)
	updateTimer();
	if (timedOut) {
#ifdef DEBUG
	dumpBoard(board);
	log << "Time out!!!\n";
	log.flush();
#endif
		return 1;
	}

	// Incrémente le nombre d'appels à l'algorithme de recherche
	searchCount++;
	
    int bestScore = playerIndex == 0 ? INT_MIN : INT_MAX;
    int score;
	int bestMove = 1;

 #ifdef DEBUG
	log << "Entree dans alphabeta playerIndex = " << playerIndex << " depth = " << depth << "\n";
	log.flush();
#endif

    // Si on est une feuille, renvoie la valeur de la fonction d'évaluation
    if (depth == maxDepth) {
		score = voronoiScore(board, p1, p2, playerIndex + 1);
#ifdef DEBUG
	dumpBoard(board);
	log << "Feuille playerIndex = " << playerIndex << " score = " << score << "\n";
	log.flush();
#endif
		return score;
	}

	// Génère la liste des déplacements possibles
	Pos currentPos = playerIndex == 0 ? p1 : p2;
	Pos opponentPos = playerIndex == 0 ? p2 : p1;
	AvailableMove a = getAvailableMoveMinmax(board, currentPos, opponentPos, playerIndex);

	int i;
	for (i = 0; i < a.count; i++) {
		int move = a.move[i];
		
		Pos newPos = currentPos;
		newPos.x += relativePosX[move];
		newPos.y += relativePosY[move];
		if (newPos.x == opponentPos.x && newPos.y == opponentPos.y) {
			// Ce mouvement mène à une égalité
			score = SCORE_DRAW;
#ifdef DEBUG
	log << "Egalite detectee\n";
#endif
		} else {
			// Joue le coup
			board[(newPos.y << 6) + newPos.x] = playerIndex == 0 ? '1' : '2';
			
			// Détermine le score associé à ce coup
			if (playerIndex == 0) {
				score = alphabeta(board, newPos, opponentPos, (playerIndex + 1) % 2, depth + 1, alpha, beta, maxDepth);
			} else {
				score = alphabeta(board, opponentPos, newPos, (playerIndex + 1) % 2, depth + 1, alpha, beta, maxDepth);
			}
			if (timedOut) {
				return 1;
			}
			
			// Annule le coup joué
			board[(newPos.y << 6) + newPos.x] = ' ';
		}

		if (playerIndex == 0) {
			// Joueur : maximise le score
			if (score > bestScore) {
				bestScore = score;
				bestMove = move;
			}

			// Applique la coupure beta
			if (bestScore >= beta) {
				break;
			}
			alpha = MAX(alpha, bestScore);
		} else {
			// Adversaire : minimise le score
			if (score < bestScore) {
				bestScore = score;
				bestMove = move;
			}

			// Applique la coupure alpha
			if (bestScore <= alpha) {
				break;
			}
			beta = MIN(beta, bestScore);
		}
	}

    if (depth == 0) {
        // Finalement, retourne le meilleur coup (/!\ pas le score)
#ifdef DEBUG
	log << "Sortie de alphabeta playerIndex = " << playerIndex << " depth = " << depth << " bestScore = " << bestScore << " bestMove = " << bestMove << "\n";
	log.flush();
#endif
		return bestMove;
    }

	// Cul de sac
	// Tente quand même de privilégier les culs de sac qui sont le plus loin dans l'arbre de recherche, en espérant que l'autre joueur va gaffer avant...
	if (bestScore == INT_MIN) {
#ifdef DEBUG
	log << "Cul de sac\n";
	log.flush();
#endif
		//bestScore = -50 * 50;
		bestScore = INT_MIN + depth;
	}
#ifdef DEBUG
	dumpBoard(board);
	log << "Sortie de alphabeta playerIndex = " << playerIndex << " depth = " << depth << " bestScore = " << bestScore << "\n";
	log.flush();
#endif

    return bestScore;
}
