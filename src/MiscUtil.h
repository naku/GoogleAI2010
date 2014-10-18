#include <iostream>
#include <fstream>

#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

using namespace std;

#ifdef DEBUG
	extern ofstream log;
#endif
#ifdef TIMER
	extern ofstream log;
#endif

extern int randomDir[4];
struct Map;

/**
 * Nombre d'appel à la fonction de recherche (à des fins d'optimisation).
 */
extern int searchCount;

/**
 * Position.
 */
struct Pos {
	int x;
	int y;
};

/**
 * Liste de mouvements disponibles.
 */
struct AvailableMove {
	// Liste des mouvements
	int move[4];
	
	// Nombre de mouvements
	int count;
};

/**
 * Randomise un tableau d'entiers.
 *
 * @param t Tableau d'entiers
 * @param size Taille du tableau
 */
void shuffle(int *t, int size);

/**
 * Déplacement relatif en X (UNUSED, N, E, S, W).
 */
extern int relativePosX[5];

/**
 * Déplacement relatif en Y (UNUSED, N, E, S, W).
 */
extern int relativePosY[5];

/**
 * Retourne une position relative à une position initiale <p>.
 *
 * @param p position initiale
 * @param dir direction (1..4 = N,E,S,O)
 * @return Position relative
 */
Pos relativePos(Pos p, int dir);

/**
 * Retourne la liste des mouvements disponibles à partir d'une position.
 *
 * @param map Carte
 * @param p Position
 * @return Liste des mouvements disponibles
 */
AvailableMove *getAvailableMove(const Map& map, Pos p);

/**
 * Retourne la liste des mouvements disponibles à partir d'une position.
 * /!\ Ni l'adversaire, ni le joueur ne comptent comme un mouvement possible.
 *
 * @param board Carte
 * @param p Position
 * @return Liste des mouvements disponibles
 */
AvailableMove getAvailableMove(const unsigned char *board, Pos p);

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
AvailableMove getAvailableMoveMinmax(const unsigned char *board, Pos p1, Pos p2, int playerIndex);

/**
 * Copie un plateau dans un autre (déjà alloué).
 *
 * @param board1 Tableau à copier
 * @param board2 Tableau cible (déjà alloué).
 */
void copyBoard(unsigned char *board1, unsigned char *board2);

/**
 * Affiche le tableau fourni en entrée sur les logs.
 *
 * @param board Plateau
 */
void dumpBoard(const unsigned char *board);

/**
 * Retourne vrai si le mouvement est impossible sur cette position.
 *
 * @param board Tableau
 * @param x Position x
 * @param y Position y
 * @param goToPlayer1 Autorise un déplacement sur la position du joueur 1
 * @param goToPlayer2 Autorise un déplacement sur la position du joueur 2
 */
bool isWall(const unsigned char *board, int x, int y, bool goToPlayer1, bool goToPlayer2);

/**
 * Initialise la profondeur de l'arbre de recherche, en fonction de la taille du terrain.
 * (Tests de calibrage faits à l'arrache).
 *
 * @param maxDepth Profondeur maximale (modifiée par effet de bord)
 */
void initMaxDepth(int *maxDepth);
