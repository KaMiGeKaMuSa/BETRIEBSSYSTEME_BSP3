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

/*
 *
 */
data_collect createSegment(int shm_size, int shm_mode) {
    data_collect ret_object;
	int shm_flag = 0;
	
	/* default values */
	ret_object.shm_size = shm_size;
	ret_object.use_mode = shm_mode;
	ret_object.sem_r = -1;
	ret_object.sem_w = -1;
	ret_object.shmid = -1;
	ret_object.segment = NULL;
	
	/*  create segment: */
	if ((ret_object.shmid = shmget(SHM_KEY, sizeof(int) * ret_object.shm_size, 0660 | IPC_CREAT)) == -1) {
		fprintf(stderr,"%s: %s\n", "shmget()", strerror(errno));
		closeSegment(ret_object);
		return ret_object;
    }

    /* attach to the segment to get a pointer to it: */
    if (ret_object.use_mode == READ_MODE) shm_flag = SHM_RDONLY;
	
    ;
    if ((ret_object.segment = shmat(ret_object.shmid, NULL, shm_flag)) == (char *)(-1)) {
		fprintf(stderr,"%s: %s\n", "shmat()", strerror(errno));
		closeSegment(ret_object);
        return ret_object;
    }
	
	/* read-semaphore don't exist, so initialize: */
	if ((ret_object.sem_r = seminit(SEM_R_KEY, 0600, ret_object.shm_size)) == -1) {
		if (errno == EEXIST) {
			/* already created by other process */
			if ((ret_object.sem_r = semgrab(SEM_R_KEY)) == -1) {
				fprintf(stderr,"%s: %s\n", "semgrab(sem_r)", strerror(errno));
				closeSegment(ret_object);
				return ret_object;
			}
		} else {
			fprintf(stderr,"%s: %s\n", "seminit(sem_r)", strerror(errno));
			closeSegment(ret_object);
			return ret_object;
		}
	}

	/* write-semaphore don't exist, so initialize: */
	if ((ret_object.sem_w = seminit(SEM_W_KEY, 0600, ret_object.shm_size)) == -1) {
		if (errno == EEXIST) {
			/* already created by other process */
			if ((ret_object.sem_w = semgrab(SEM_W_KEY)) == -1) {
				fprintf(stderr,"%s: %s\n", "semgrab(sem_w)", strerror(errno));
				closeSegment(ret_object);
				return ret_object;
			}
		} else {
			fprintf(stderr,"%s: %s\n", "seminit(sem_w)", strerror(errno));
			closeSegment(ret_object);
			return ret_object;
		}
	}
	
	return ret_object;
}
    
/**
 * -------------------------------------------------------------- close segment - function --
 */
int closeSegment(data_collect shm_sem) {
    int returnvalue = 0;
    
    /* clean up semaphores */
	if (shm_sem.sem_w != -1) {
		if ((returnvalue = semrm(shm_sem.sem_w)) == -1) {
			fprintf(stderr, "%s: %s\n", "semrm(shm_sem.sem_w)", strerror(errno));
			shm_sem.sem_w = -1;
			return returnvalue;
		}
	}
	
	if (shm_sem.sem_r != -1) {
		if ((returnvalue = semrm(shm_sem.sem_r)) == -1) {
			fprintf(stderr, "%s: %s\n", "semrm(shm_sem.sem_r)", strerror(errno));
			shm_sem.sem_r = -1;
			return returnvalue;
		}
	}
	
    /*  detach shared memory segment: */
	if (shm_sem.segment != NULL) {
		if ((returnvalue = shmdt(shm_sem.segment)) == -1) {
			fprintf(stderr, "%s: %s\n", "shmdt()", strerror(errno));
			shm_sem.segment = NULL;
			return returnvalue;
		}
		shm_sem.segment = NULL;
	}
	
    /*  if shared memory segment exists mark as removable */
	if (shm_sem.shmid != -1) {
		if ((returnvalue = shmctl(shm_sem.shmid, IPC_RMID, NULL)) == -1) {
			fprintf(stderr, "%s: %s\n", "shmctl()", strerror(errno));
			shm_sem.shmid = -1;
			return returnvalue;
		} 
		shm_sem.shmid = -1;
	}

    return returnvalue;
}

/**
 * -------------------------------------------------------------- getopt - function --
 */
int parseParameter(int argc, char * argv[]) {
	int option;
	long int size = -1;
	char* prest = NULL;

	errno = 0;

	/* check parameter list for valid options */
	while ((option = getopt(argc, argv, "+m:")) != -1) {
		switch (option) {
			case 'm':
				/* convert buffer size string to numeric  */
				size = strtol(optarg, &prest, 10);
				if (errno != 0 || *prest != '\0' || size <= 0) {
					fprintf(stderr, "%s: %s\n", "could not convert argument", strerror(errno));
					return -1;
				}
				break;
				
			default:
				fprintf(stderr, "%s: -%c.\n", "invalid option", option);
				return -1;
		}
	}

	/* check for extra parameters */
	if (optind < argc) {
		fprintf(stderr, "%s:\n", "illegal number of options");
		return -1;
	}

	return size;
}
