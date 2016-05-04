/**
 * @file sharedfunctions.c
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
 
/**
 * -------------------------------------------------------------- create segment - function --
*/
char * createSegment(int shm_size) {
	int key = getuid()*1000;
    char *segment;
    int shmid;
    
    /*  check if segment exists: */
	if ((shmid = shmget(key, shm_size, 0666)) == -1) {
		/*  ENOENT = No segment exists for the given key, and IPC_CREAT was not specified */
		if(errno == ENOENT) {
			/*  create segment: */
			if ((shmid = shmget(key, shm_size, IPC_CREAT | IPC_EXCL)) == -1) {
				perror("shmget");
				return -1;
			}
		}
    }

    /* attach to the segment to get a pointer to it: */
    segment = shmat(shmid, (void *)0, 0);
    if (segment == (char *)(-1)) {
        perror("shmat");
        return -1;
    }
	
	return segment;
}
    
/**
 * -------------------------------------------------------------- close segment - function --
 */
int closeSegment(char *segment, int shm_size) {
	int key = getuid()*1000;
	int returnvalue;
	
	/*  detach shared memory segment: */
    if ((returnvalue = shmdt(segment)) == -1) {
        perror("shmdt");
        return returnvalue;
    }
    
	/*  check if segment exists: */
	if ((shmid = shmget(key, shm_size, 0666)) == -1) {
		if(errno != ENOENT) {
			perror("shmget");
			return shmid;
		}
	}
	/*  if shared memory segment exists mark as removable */
	if ((returnvalue = shmctl (shmid, IPC_RMID, 0)) == -1) {
		perror("shmctl: shmctl failed");
		return returnvalue;
	} 

	return returnvalue;
}

/**
 * -------------------------------------------------------------- getopt - function --
 */
int parseParameter(int argc, char **argv) {
	int ret = 0, fail = 0, c;
	
	while ((c = getopt(argc, argv, "m:")) != EOF) {
        switch (c) {
        case 'm':
            ret = atoi(optarg);
            break;
        default:
            fail=1;
            break;
        }
    }
	
	if (fail) {
        fprintf(stderr,"usage: %s [-m size] ...\n", argv[0]);
        return(fail);
    }
	
	return ret;
}
