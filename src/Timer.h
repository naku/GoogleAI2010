/**
 * Variable a tester à chaque itération de l'algorithme de recherche.
 * Indique que l'on a dépassé le temps alloué et qu'il faut retourner au plus vite la réponse...
 */
extern bool timedOut;

/**
 * Démarre le timer.
 * A appeler avant chaque coup.
 */
void startTimer();

/**
 * Mise à jour de timer.
 * A appeler à chaque itération.
 */
void updateTimer();

/**
 * Arrete de timer.
 * A appeler après chaque coup.
 */
void endTimer();
