extern int MINMAX_MAX_DEPTH;

/**
 * Recherche par approfondissement itératif.
 * Effectue une recherche en profondeur, limitée au niveau 2, puis 4, etc, jusqu'à l'obtention d'une solution.
 * Cela permet de simuler une recherche en largeur, mais sans prendre beaucoup d'espace mémoire.
 *
 * @param map Plateau
 * @return Mouvent effectué par le P1
 */
int iterativeDeepening(const Map& map);
