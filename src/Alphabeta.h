extern int ALPHABETA_MAX_DEPTH;

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
int alphabeta(unsigned char *board, Pos p1, Pos p2, int playerIndex, int depth, int alpha, int beta, int maxDepth);
