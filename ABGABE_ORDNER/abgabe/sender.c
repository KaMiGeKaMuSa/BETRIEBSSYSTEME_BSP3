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
 **/

/*
 * -------------------------------------------------------------- includes --
 */
#include "sharedmemory.h"

/*
 * -------------------------------------------------------------- global static variables --
 */

/**
 *
 * \brief sender
 *
 * Sender Programm
 *
 * A sender transmitts data to a shared memory
 *
 *
 * \param argc the number of arguments
 * \param argv the arguments itselves (including the program name in argv[0])
 *
 * \return 0 or -1
 *
 * \retval 0 (Everything OK)
 * \retval -1 (Something goes wrong)
 *
 **/

int main(int argc, char * argv[]) {
	data_collect shm_sem;
	int shm_size = 0, pos = 0;
	int input = -1;
	
	/*  get size as parameter */
    if ((shm_size = parseParameter(argc, argv)) == -1) {
		fprintf(stderr, "Usage: sender -m <ringbuffer size>\n");
		return -1;
	}
    
	/*  create segment/semaphore and return collection or when already created only return collection */
	shm_sem = createSegment(shm_size, WRITE_MODE);
	if (shm_sem.segment == NULL) return -1;
	
	do {
		/* get character from stdin */
		input = fgetc(stdin);
		
		/* Decrement write semaphore, because we write onto a free segment, if >0 all ok else block application */
		while (P(shm_sem.sem_w) == -1) {
			if (errno != EINTR) {
				fprintf(stderr, "%s: %s\n", "P(shm_sem.sem_w)", strerror(errno));
				closeSegment(shm_sem);
				return -1;
			}
			errno = 0;
		}
		
		/* and write into shared memory */
		shm_sem.segment[pos] = input;
		pos++;
		
		/* if pos >= shared memory size, start at 0 again */
		if (pos >= shm_sem.shm_size) {
			pos = 0;
		}
		
		errno = 0;
		/* increment read semaphore to tell receiver that something is ready to read */
		if (V(shm_sem.sem_r) == -1) {
			fprintf(stderr, "%s: %s\n", "V(shm_sem.sem_r)", strerror(errno));
			closeSegment(shm_sem);
			return -1;
		}
	} while (input != EOF);
	
	return 0;
}
