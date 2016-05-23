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
	int pos = 0;
	char output;
	
	/*  get size as parameter */
	shm_size = parseParameter(argc, argv);
	
	/*  create segment/semaphore and return collection or when already created only return collection */
	shm_sem = createSegment(shm_size);
	
	
	// 3) while (!= EOF) von Shared Memory lesen
	//		aufpassen das Leseindex hinter Schreibindex bleibt
	do {
		/* Decrement read semaphore, because we read from a written segment, if >0 all ok else block application */
		if (P(shm_sem.sem_r) != 0) {
			if (errno == EINTR) {
				/* syscall interrupted by signal, try again */
				continue;
			}
			perror("P(shm_sem.sem_r)");
			closeSegment(shm_sem);
			break;
		}
	  
		/* get character from shared memory and write to stdout */
		output = shm_sem.segment[pos];
		pos++;
		
		if (output != EOF) {
			printf("%c", output);
		}
		
		/* if pos == shared memory size, start at 0 again */
		if (pos == shm_sem.shm_size) {
			pos = 0;
		}
		
		/* increment write semaphore to tell sender that segment is read and can be overwritten */
		if (V(shm_sem.sem_w) != 0) {
			perror("V(shm_sem.sem_w)");
			closeSegment(shm_sem);
			break;
		}
	} while (output != EOF);

	return 0;
}
