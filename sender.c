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
int main(int argc, char * argv[]) {
	data_collect shm_sem;
	int shm_size = 0, pos = 0;
	char input;
	
	/*  get size as parameter */
    shm_size = parseParameter(argc, argv);
    
	/*  create segment/semaphore and return collection or when already created only return collection */
	shm_sem = createSegment(shm_size);
    
    
	// 3) while (!= EOF) auf Shared Memory schreiben
	//		aufpassen das Schreibindex hinter Leseindex bleibt
	do {
		/* Decrement write semaphore, because we write onto a free segment, if >0 all ok else block application */
		if (P(shm_sem.sem_w) != 0) {
			if (errno == EINTR) {
				/* syscall interrupted by signal, try again */
				continue;
			}
			perror("P(shm_sem.sem_w)");
			closeSegment(shm_sem);
			break;
		}
	  
		/* get character from stdin and write into shared memory */
		input = fgetc(stdin);
		shm_sem.segment[pos] = input;
		pos++;
		
		/* if pos == shared memory size, start at 0 again */
		if (pos == shm_sem.shm_size) {
			pos = 0;
		}
		
		/* increment read semaphore to tell receiver that something is ready to read */
		if (V(shm_sem.sem_r) != 0) {
			perror("V(shm_sem.sem_r)");
			closeSegment(shm_sem);
			break;
		}
	} while (input != EOF);
	
	return 0;
}
