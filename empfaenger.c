/**
 * @file empfaenger.c
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
	data_collect shm_sem;
	int shm_size = 0;
	int sendData;
	int pos;
	
	/*  get size as parameter */
	shm_size = parseParameter(argc, argv);
	
	/*  create segment/semaphore and return collection or when already created only return collection */
	shm_sem = createSegment(shm_size);
	
	
	// 3) while (!= EOF) von Shared Memory lesen
	//		aufpassen das Leseindex hinter Schreibindex bleibt

	return 0;
}
