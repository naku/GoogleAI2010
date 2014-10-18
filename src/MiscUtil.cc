#include <stdio.h>
#include <stdlib.h>
#include "MiscUtil.h"
#include "Map.h"
#include <string.h>

using namespace std;

#ifdef DEBUG
	ofstream log;
#endif
#ifdef TIMER
	ofstream log;
#endif

int randomDir[4] = {1, 2, 3, 4};

/**
 * Nombre d'appel à la fonction de recherche (à des fins d'optimisation).
 */
int searchCount;

/**
 * Randomise un tableau d'entiers.
 *
 * @param t Tableau d'entiers
 * @param size Taille du tableau
 */
void shuffle(int *t, int size) {
	for (int i = 0; i < size; i++) {
		int pos = rand() % size;
		int tmp = t[i];
		t[i] = t[pos];
		t[pos] = tmp;
	}
}

/**
 * Déplacement relatif en X (UNUSED, N, E, S, W).
 */
int relativePosX[5] = {0, 0, 1, 0, -1};

/**
 * Déplacement relatif en Y (UNUSED, N, E, S, W).
 */
int relativePosY[5] = {0, -1, 0, 1, 0};

/**
 * Retourne une position relative à une position initiale <p>.
 *
 * @param p position initiale
 * @param dir direction (1..4 = N,E,S,O)
 * @return Position relative
 */
/*Pos relativePos(Pos p, int dir) {
	p.x += relativePosX[dir];
	p.y += relativePosY[dir];
	return p;
}*/

/**
 * Retourne vrai si le mouvement est impossible sur cette position.
 *
 * @param board Tableau
 * @param x Position x
 * @param y Position y
 * @param goToPlayer1 Autorise un déplacement sur la position du joueur 1
 * @param goToPlayer2 Autorise un déplacement sur la position du joueur 2
 */
bool isWall(const unsigned char *board, int x, int y, bool goToPlayer1, bool goToPlayer2) {
	if (x < 0 || y < 0 || x >= map->map_width || y >= map->map_height) {
		return true;
	} else {
		unsigned char c = board[(y << 6) + x];
		if (c == ' ') {
			return false;
		} else if (goToPlayer1 && c == '1') {
			return false;
		} else if (goToPlayer2 && c == '2') {
			return false;
		}
	}
	return true;
}

/**
 * Retourne vrai si le mouvement est impossible sur cette position.
 * /!\ Autorise un déplacement sur p1 (cette fonction doit être appelée uniquement pour p2).
 *
 * @param board Tableau
 * @param x Position x
 * @param y Position y
 * @param p1 Position du joueur 1
 */
bool isWallMinmax(const unsigned char *board, int x, int y, Pos p1) {
	if (x < 0 || y < 0 || x >= map->map_width || y >= map->map_height) {
		return true;
	} else {
		unsigned char c = board[(y << 6) + x];
		if (c == ' ') {
			return false;
		} else if (c == '1' && x == p1.x && y == p1.y) {
			return false;
		}
	}
	return true;
}

/**
 * Retourne la liste des mouvements disponibles à partir d'une position.
 * /!\ L'adversaire compte comme une position disponible
 * /!\ Le joueur compte comme une position disponible
 *
 * @param map Carte
 * @param p Position
 * @return Liste des mouvements disponibles
 */
AvailableMove *getAvailableMove(const Map& map, Pos p) {
	AvailableMove *a = (AvailableMove *) malloc(sizeof(AvailableMove));
	a->count = 0;
	
	for (int i = 1; i <= 4; i++) {
		Pos p2 = p;
		p2.x += relativePosX[i];
		p2.y += relativePosY[i];
		
		if (!map.IsWall(p2.x, p2.y)) {
#ifdef DEBUG
//	log << "Available X = " << p2.x << " Y = " << p2.y << "\n";
//	log.flush();
#endif		
			a->move[a->count] = i;
			a->count++;
		}
	}
	
	return a;
}

/**
 * Retourne la liste des mouvements disponibles à partir d'une position.
 * /!\ Ni l'adversaire, ni le joueur ne comptent comme un mouvement possible.
 *
 * @param board Carte
 * @param p Position
 * @return Liste des mouvements disponibles
 */
AvailableMove getAvailableMove(const unsigned char *board, Pos p) {
	AvailableMove a;
	a.count = 0;
	
	for (int i = 1; i <= 4; i++) {
		Pos p2 = p;
		p2.x += relativePosX[i];
		p2.y += relativePosY[i];

		// ** DEBUG optim inline **
		bool isWall = true;
		if (p2.x < 0 || p2.y < 0 || p2.x >= map->map_width || p2.y >= map->map_height) {
			// NOP
		} else {
			unsigned char c = board[(p2.y << 6) + p2.x];
			if (c == ' ') {
				isWall = false;
			}
		}
		// ** FIN optim inline **

		//if (!isWall(board, p2.x, p2.y, false, false)) {
		if (!isWall) {
#ifdef DEBUG
//	log << "Available X = " << p2.x << " Y = " << p2.y << "\n";
//	log.flush();
#endif		
			a.move[a.count] = i;
			a.count++;
		}
	}
	
	return a;
}

/**
 * Retourne la liste des mouvements disponibles à partir d'une position.
 * /!\ C'est toujours p1 Qui bouge. En revanche, si playerIndex == 1, la position de p1 est disponible.
 *
 * @param board Carte
 * @param p1 Position du joueur qui bouge
 * @param p2 Position du joueur adversaire
 * @param playerIndex Indice du joueur qui bouge
 * @return Liste des mouvements disponibles
 */
AvailableMove getAvailableMoveMinmax(const unsigned char *board, Pos p1, Pos p2, int playerIndex) {
	AvailableMove a;
	a.count = 0;
	
	for (int i = 1; i <= 4; i++) {
		Pos p1New = p1;
		p1New.x += relativePosX[i];
		p1New.y += relativePosY[i];
		if ((playerIndex == 0 && !isWall(board, p1New.x, p1New.y, false, false)) || (playerIndex == 1 && !isWallMinmax(board, p1New.x, p1New.y, p2))) {
#ifdef DEBUG
//	log << "Available X = " << p1New.x << " Y = " << p1New.y << "\n";
//	log.flush();
#endif		
			a.move[a.count] = i;
			a.count++;
		}
	}
	
	return a;
}

/**
 * Copie un plateau dans un autre (déjà alloué).
 *
 * @param board1 Tableau à copier (source)
 * @param board2 Tableau cible (déjà alloué).
 */
void copyBoard(unsigned char *board1, unsigned char *board2) {
	memcpy(board2, board1, map->map_height * 64 * sizeof(unsigned char));
}

/**
 * Affiche le tableau fourni en entrée sur les logs.
 *
 * @param board Plateau
 */
void dumpBoard(const unsigned char *board) {
#ifdef DEBUG
	for (int j = 0; j < map->map_height; j++) {
		for (int i = 0; i < map->map_width; i++) {
			char c = board[(j << 6) + i];
			log << c;
		}
		log << "\n";
	}
	log.flush();
#endif
}

/**
 * Initialise la profondeur de l'arbre de recherche, en fonction de la taille du terrain.
 * (Tests de calibrage faits à l'arrache).
 *
 * @param maxDepth Profondeur maximale (modifiée par effet de bord)
 */
void initMaxDepth(int *maxDepth) {
	int size = map->map_height * map->map_width;
	if (size <= 14 * 14) {
		*maxDepth = 10;
	} else if (size <= 16 * 16) {
		*maxDepth = 8;
	} else {
		*maxDepth = 4;
	}
	/*if (size <= 16 * 16) {
		*maxDepth = 12;
	} else if (size <= 30 * 30) {
		*maxDepth = 6;
	} else {
		*maxDepth = 4;
	} */
#ifdef DEBUG
	log << "Initialisation de la profondeur max. = " << *maxDepth << "\n";
	log.flush();
#endif	
}
