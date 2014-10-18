/**
 * Variable a tester � chaque it�ration de l'algorithme de recherche.
 * Indique que l'on a d�pass� le temps allou� et qu'il faut retourner au plus vite la r�ponse...
 */
extern bool timedOut;

/**
 * D�marre le timer.
 * A appeler avant chaque coup.
 */
void startTimer();

/**
 * Mise � jour de timer.
 * A appeler � chaque it�ration.
 */
void updateTimer();

/**
 * Arrete de timer.
 * A appeler apr�s chaque coup.
 */
void endTimer();
