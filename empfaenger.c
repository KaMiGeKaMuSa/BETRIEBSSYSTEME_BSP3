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
int main(int argc, char * argv[]) {
	data_collect shm_sem;
	int shm_size = 0;
	int pos = 0;
	int output = -1;
	
	/*  get size as parameter */
	if ((shm_size = parseParameter(argc, argv)) == -1) {
		return -1;
	}
	
	/*  create segment/semaphore and return collection or when already created only return collection */
	shm_sem = createSegment(shm_size, READ_MODE);
	if (shm_sem.segment == NULL) return -1;
	
	do {
		/* Decrement read semaphore, because we read from a written segment, if >0 all ok else block application */
		while (P(shm_sem.sem_r) == -1) {
			if (errno != EINTR) {
				fprintf(stderr, "%s: %s\n", "P(shm_sem.sem_r)", strerror(errno));
				closeSegment(shm_sem);
				return -1;
			}
			errno = 0;
		}
	  
		/* get character from shared memory and write to stdout */
		output = shm_sem.segment[pos];
		pos++;
		
		if (output != EOF) {
			if (fputc(output, stdout) == EOF) {
				fprintf(stderr, "%s: %s\n", "printf()", strerror(errno));
				closeSegment(shm_sem);
				return -1;
			}
		}
		
		/* if pos >= shared memory size, start at 0 again */
		if (pos >= shm_sem.shm_size) {
			pos = 0;
		}
		
		/* increment write semaphore to tell sender that segment is read and can be overwritten */
		if (V(shm_sem.sem_w) != 0) {
			fprintf(stderr, "%s: %s\n", "V(shm_sem.sem_w)", strerror(errno));
			closeSegment(shm_sem);
			return -1;
		}
	} while (output != EOF);
	
	closeSegment(shm_sem);

	return 0;
}
