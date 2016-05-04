/**
 * @file sender.c
 * sharedmemory
 * Beispiel 3
 *
 * @author Karin Kalman <karin.kalman@technikum-wien.at>
 * @author Michael Mueller <michael.mueller@technikum-wien.at>
 * @author Gerhard Sabeditsch <gerhard.sabeditsch@technikum-wien.at>
 * @date 2016/04/17
 *
 * @version $Revision: 1 $
 *
 *
 * URL: $HeadURL$
 *
 * Last Modified: $Author: Gerhard $
 */

/**
 * -------------------------------------------------------------- includes --
 */
#include "sharedmemory.h"

/**
 * -------------------------------------------------------------- global static variables --
 */

 /*
 * ------------------------------------------------------------- main --
 */
int main(int argc, const char * argv[]) {
	char *segment = NULL;
	int shm_size = 0;
	
	/*  get size as parameter */
	shm_size = parseParameter(argc, argv);

	/*  create segment or when already created return segment */
	segment = createSegment(shm_size);
	
	// 2) Methode um Semaphoren anzulegen aufrufen
	// 3) while (!= EOF) auf Shared Memory schreiben
	//		aufpassen das Schreibindex hinter Leseindex bleibt
	
	return 0;
}
