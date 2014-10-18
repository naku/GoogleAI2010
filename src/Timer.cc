#include "Timer.h"
#include "MiscUtil.h"
#include <signal.h>
#include <sys/time.h>
#include <time.h>

int TIMER_TIMEOUT_US = 990000;
//int TIMER_TIMEOUT_US = 990000;

/**
 * Variable a tester � chaque it�ration de l'algorithme de recherche.
 * Indique que l'on a d�pass� le temps allou� et qu'il faut retourner au plus vite la r�ponse...
 */
bool timedOut = false;

itimerval timeoutTimer;

timeval timeStart;
timeval timeNow;
struct timezone tz;

/**
 * D�marre le timer.
 * A appeler avant chaque coup.
 */
void startTimer() {
	timedOut = false;

	gettimeofday(&timeStart, &tz);
}

/**
 * Mise � jour de timer.
 * A appeler � chaque it�ration.
 */
void updateTimer() {
	if (timedOut) {
		return;
	}
	gettimeofday(&timeNow, &tz);
	
	int lapsedS = timeNow.tv_sec - timeStart.tv_sec;
	int lapsedUs = timeNow.tv_usec - timeStart.tv_usec;
	lapsedUs += 1000000 * lapsedS;
	if (lapsedUs > TIMER_TIMEOUT_US) {
		timedOut = true;
	}
}

/**
 * Arrete de timer.
 * A appeler apr�s chaque coup.
 */
void endTimer() {
	updateTimer();
#ifdef TIMER
	int lapsedMs = timeNow.tv_usec - timeStart.tv_usec;
	if (lapsedMs < 0) {
		lapsedMs += 1000000;
	}
	log << "Timer begin=" << timeStart.tv_sec << ":" << timeStart.tv_usec << " end=" << timeNow.tv_sec << ":" << timeNow.tv_usec << " Dur�e " << lapsedMs << " ms\n";
	if (timedOut) {
		log << "Timed out!\n";
	}
	log.flush();
#endif
}
