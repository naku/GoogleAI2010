extern int MINMAX_MAX_DEPTH;

/**
 * Recherche par approfondissement it�ratif.
 * Effectue une recherche en profondeur, limit�e au niveau 2, puis 4, etc, jusqu'� l'obtention d'une solution.
 * Cela permet de simuler une recherche en largeur, mais sans prendre beaucoup d'espace m�moire.
 *
 * @param map Plateau
 * @return Mouvent effectu� par le P1
 */
int iterativeDeepening(const Map& map);
