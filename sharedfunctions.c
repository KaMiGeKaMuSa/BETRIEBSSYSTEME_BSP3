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
data_collect createSegment(int shm_size) {
    data_collect ret_object;
	
	ret_object.shm_size = shm_size;
	
    /*  check if segment exists: 0666 == look if exist*/
	if ((ret_object.shmid = shmget(SHM_KEY, ret_object.shm_size, 0666)) == -1) {
		/*  ENOENT = No segment exists for the given key, and IPC_CREAT was not specified */
		if(errno == ENOENT) {
			/*  create segment: */
			if ((ret_object.shmid = shmget(SHM_KEY, ret_object.shm_size, IPC_CREAT | IPC_EXCL)) == -1) {
				perror("shmget");
				ret_object.segment = NULL;
				return ret_object;
			}
		}
    }

    /* attach to the segment to get a pointer to it: */
    ret_object.segment = shmat(ret_object.shmid, (void *)0, 0);
    if (ret_object.segment == (char *)(-1)) {
        perror("shmat");
		ret_object.segment = NULL;
        return ret_object;
    }
	
	/* check if read-semaphore exists: */
	if ((ret_object.sem_r = semgrab(SEM_R_KEY)) == -1) {
		/* read-semaphore don't exist, so initialize: */
		if ((ret_object.sem_r = seminit(SEM_R_KEY, 0600, 0)) == -1) {
			perror("seminit(sem_r)");
			ret_object.segment = NULL;
			return ret_object;
		}
	}
	/* check if write-semaphore exists: */
	if ((ret_object.sem_w = semgrab(SEM_W_KEY)) == -1) {
		/* write-semaphore don't exist, so initialize: */
		if ((ret_object.sem_w = seminit(SEM_W_KEY, 0600, ret_object.shm_size)) == -1) {
			perror("seminit(sem_w)");
			ret_object.segment = NULL;
			return ret_object;
		}
	}
	
	
	return ret_object;
}
    
/**
 * -------------------------------------------------------------- close segment - function --
 */
int closeSegment(data_collect shm_sem) {
    int returnvalue;
    
    /* clean up semaphores */
    if ((returnvalue = semrm(shm_sem.sem_w)) == -1) {
	perror("semrm(shm_sem.sem_w)");
	return returnvalue;
    }
    if ((returnvalue = semrm(shm_sem.sem_r)) == -1) {
	perror("semrm(shm_sem.sem_r)");
	return returnvalue;
    }
	
    /*  detach shared memory segment: */
    if ((returnvalue = shmdt(shm_sem.segment)) == -1) {
        perror("shmdt");
        return returnvalue;
    }
    
    /*  detach shared memory segment: */
    if ((returnvalue = shmdt(shm_sem.segment)) == -1) {
        perror("shmdt");
        return returnvalue;
    }
    
    /*  check if segment exists: */
    if ((shm_sem.shmid = shmget(SHM_KEY, shm_sem.shm_size, 0666)) == -1) {
	if(errno != ENOENT) {
		perror("shmget");
		return shm_sem.shmid;
	}
    }
	
    /*  if shared memory segment exists mark as removable */
    if ((returnvalue = shmctl(shm_sem.shmid, IPC_RMID, 0)) == -1) {
	perror("shmctl: shmctl failed");
	return returnvalue;
    } 

    return returnvalue;
}

/**
 * -------------------------------------------------------------- getopt - function --
 */
int parseParameter(int argc, char *argv[]) {
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
        return -1;
    }
	
	return ret;
}
