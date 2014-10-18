#include <iostream>
#include <fstream>

/**
 * Calcule le score associ� au "territoire de Voronoi" d'un plateau.
 * Le joueur 2 bouge en premier.
 *
 * @param board Plateau
 * @param p1 Position initiale du joueur 1
 * @param p2 Position initiale du joueur 2
 * @param playerIndex Indice du joueur du point de vue duquel on �value le score (0, 1)
 */
int voronoiScore(unsigned char *board, Pos p1, Pos p2, int playerIndex);

/**
 * Score pour une �galit�.
 */
extern int SCORE_DRAW;

/**
 * A appeler une fois en d�but de coup pour allouer les variables temporaires de Voronoi.
 */
void initVoronoi();


/**
 * A appeler en fin de coup pour lib�rer les variables temporaires de Voronoi.
 */
void freeVoronoi();
