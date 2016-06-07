/**
 * @file empfaenger.c
 * Betriebssysteme IPC.
 * Beispiel 3
 *
 * @author Benjamin Petterle <ic15b077@technikum-wien.at>
 * @author Matthias Greil <ic15b006@technikum-wien.at>
 * @author Oguz Kaya <ic15b055@technikum-wien.at>
 * @date 2016/06/02
 *
 * @version 1.0
 *
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <error.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sem182.h>
#include <limits.h>

/*
 * --------------------------------------------------------------- defines --
 */
/* ### FB: Sollen nicht konstant sein, sondern mittels  getuid(2) für jeden User eigene Keys anlegen: */
#define SHMKEY 8947345
#define SEMKEY1 37489174
#define SEMKEY0 943057
/*
 * -------------------------------------------------------------- typedefs --
 */
/*
 * --------------------------------------------------------------- globals --
 */
/*
 * ------------------------------------------------------------- functions --
 */
void startEmpfaenger(int parameter);
int removeSemaphores(int semid0, int semid1);
int removeSharedMemory(int *shmptr, int shmid);
void clean(int semid0, int semid1, int *shmptr, int shmid);
int print(const char * string);
void usage(void);


/**
 *
 * @brief main Funktion mit getopt
 *
 * Liest mit getopt Argumente ein, überprüft diese und ruft die Funktion startEmpfaenger()
 * 
 * @param argc the number of arguments
 * @param argv the arguments itselves (including the program name in argv[0])
 *
 * @return 0 on success and 1 on error
 * @retval 0 success
 *  	   1 error
 *		
 */
int main(int argc, char *argv[])
{
	int opt;
	int parameter = 0;
	char *endptr;
	opterr = 0;

	/* ist ausschalggebend dafür ob ein Fehler passiert is oder nicht */
	int doit = -1;

	while ((opt = getopt(argc, argv, "m:")) != -1)
	{
		switch (opt)
		{
			case 'm':
				errno = 0;
				parameter = strtol(optarg, &endptr, 10);
				if ((errno == ERANGE && (parameter == LONG_MAX || parameter == LONG_MIN)) || (errno != 0 && parameter == 0)) {
			        doit = -1;
			        break;
			    }

			    if (*endptr != '\0') {
			    	doit = -1;
			    	break;
			    }

				doit = 1;
				break;
			default:
				doit = -1;
				break;
		}
		/* unterbricht die While schleife falls ein Fehler passiert is */
		if(doit == -1)
			break;
	}
	/* überprüft ob die richtige anzahl von parametern übergeben wurde */
	if(optind != argc) {
		doit = -1;
	}

	/* wenn kein Fehler passiert ist wird das Programm ausgeführt und nach der Ausführung beendet */
	if(doit == 1){
		startEmpfaenger(parameter);
		(void)fflush(stdout);
		return 0;
	} 
	/* Falls ein Fehler passiert ist wird die Usagemitteilung ausgegeben und das Programm beendet */ 
	usage();
	(void)fflush(stdout);
	return 1;
}

/**
 *
 * @brief Funktion zur Erstellung der Semaphore, des Shared Memorys und Empfang von Daten
 *
 * Versucht sich zu einem bereits bestehenden Shared Memory zu attachen, schlägt dies fehl wird ein neues Shared Memory angelegt
 * Versucht neue Semaphoren zu erstellen und zu initialisieren, schlägt dies fehl wird versucht auf ein bereits vorhandenes Semaphore zuzugreifen
 * Gibt es einen Fehler bei Shared Memory attach oder anlegen bzw. Semaphore initalisierung oder zugreifen, wird die clean Funktion aufgerufen und das Programm beendet.
 * Auslesen des Ringbuffers bis End Of File
 * Ausgabe der empfangenen Zeichen auf stdin
 * 
 *
 * @param parameter Anzahl der Ringbufferelemente

 */
void startEmpfaenger(int parameter){
	/* ### FB: Alle Variablen am Anfang der Funktion deklarieren */
	/* legt den Shared Memory an oder nimmt ihn falls vorhanden */
	const int shmid = shmget(SHMKEY, parameter * sizeof(int), 0660 | IPC_CREAT);
	if (shmid == -1)
	{
		perror("Error creating/getting shared memory!");
	}

	/* attached den Shared Memory */
	int *shmptr = shmat(shmid, NULL, SHM_RDONLY);
	if (shmptr == (int *)-1)
	{
		perror("Error attaching shared memory!");
	}
	
	/* legt die erste semaphore an falls nicht vorhanden */
	int semid1 = seminit(SEMKEY1, 0660, 0);
	if(semid1 == -1)
	{
		/* falls vorhanden nimm vorhandene */
		semid1 = semgrab(SEMKEY1);
		if(semid1 == -1)
		{
			perror("Error creating a Semaphore");
		}
	}
	
	/* legt die zweite semaphore an falls nicht vorhanden*/
	int semid0 = seminit(SEMKEY0, 0660, parameter);
	if(semid0 == -1)
	{
		/* falls vorhanden nimm vorhandene */
		semid0 = semgrab(SEMKEY0);
		if(semid0 == -1)
		{
			perror("Error creating a Semaphore");
		}
	}

	/* ### FB: Warum erst hier clean+exit aufrufen und nicht in jeder Fehlerbehandlung der Codezeilen weiter oben? Sobald ein Fehler auftritt, muss nicht mehr versucht werden alles andere zu erstellen */
	/* Wenn eine der angelegten semaphoren oder das shared memory nicht initialisiert ist, gib alles wieder frei */
	if(semid1 == -1 || semid0 == -1 || shmid == -1 || *shmptr == -1) 
	{
		clean(semid0, semid1, shmptr, shmid);
		exit(1);		
	}
	
	int emp=0;
	int * a = shmptr;
	int i = 0;

	while(1)
	{
		/* warten auf semaphore */
		while(P(semid1) < 0)
		{
			/* falls EINTR dann nochmals p */
			if(errno == EINTR)
				continue;

			/* sonst beenden und aufräumen */
			perror("error waiting for Semaphore");
			clean(semid0, semid1, shmptr, shmid);
			exit(1);
		}
		emp = *a;
		/* semaphore signalisieren */
		while(V(semid0) < 0)
		{
			/* falls EINTR dann nochmals v */
			if(errno == EINTR)
				continue;

			/* sonst beenden und aufräumen */
			perror("error freeing Semaphore");
			clean(semid0, semid1, shmptr, shmid);
			exit(1);
		}
		a++;
		i++;
		if(i >= parameter)
		{
			a = shmptr;
			i=0;
		}
		
		if(emp == EOF)
			break;
		else
		{
			if(fprintf(stdout, "%c", emp) < 0)
			{
				/* ### FB: Alle Fehlermeldungen sind auf Englisch bis auf folgende: */
				perror("Fehler beim schreiben auf STDIN");
				break;
			}
			
		}
	}
	
	/* aufräumen bei beenden des Programms */
	clean(semid0, semid1, shmptr, shmid);
}


/**
 *
 * @brief usage Funktion
 *
 * Gibt die Bedientungsanleitung für das Programm auf stderr aus 
 * 
 */
void usage(void)
{
	/* Keine Fehlerbehandlung notwendig, da nach usage das Programm exekutiert wird */
	(void)fprintf(stderr, "%s\n", "usage: empfaenger -m <value>");
}

/**
 *
 * @brief usage Funktion
 *
 * Gibt den String aus und macht Fehlerbehandlung
 *
 * @param string Auszugebender String
 * 
 * @return 0 on success and 1 on error
 * @retval 0 success
 *  	   -1 error
 */
 /* ### FB: Funktion wird nirgends verwendet? */
int print(const char * string)
{
	errno = 0;
	if(printf("%s", string) < 0)
	{
		error(0, errno, NULL);
		return -1;
	}
	return 0;
}

/**
 *
 * @brief Funktion zum Freigeben und Löschen des Shared Memorys
 *
 * Ruft die Funktion shmdt auf um den Shared Memory freizugeben, überprüft auf Fehler
 * Ruft anschließend shmctl auf um den Shared Memory zu löschen, überprüft auf Fehler
 *
 * @param  shmptr Pointer auf Shared Memory
 * @param  shmid ID des Shared Memory
 * 
 * @return 0 on success and 1 on error
 * @retval 0 success
 *  	   -1 error
 */
int removeSharedMemory(int *shmptr, int shmid)
{
	errno = 0;
	int retval = 0;
	if (shmdt(shmptr) == -1)
	{
		error(0, errno, NULL);
		retval = -1;
	}
	if(errno != EIDRM && shmctl(shmid, IPC_RMID, NULL) == -1) 
	{	
		error(0, errno, NULL);
		retval = -1;
	}
	
	return retval;
}

/**
 *
 * @brief Funktion zum Löschen der Semaphore
 *
 * Ruft die Funktion semrem auf um die Semaphore zu löschen, überprüft
 *
 * @param  semid0 ID der 1. Semaphore
 * @param  semid1 ID der 2. Semaphore
 * 
 * @return 0 on success and 1 on error
 * @retval 0 success
 *  	   -1 error
 */
int removeSemaphores(int semid0, int semid1)
{
	int retval = 0;
	/* ### FB: Kommentar im falschen Format und nicht verständlich: */
	if(semid0 != -1 && semrm(semid0) == -1 ) // nur einer von beiden
	{
		retval = -1;
	}
	if(semid1 != -1 && semrm(semid1) == -1)
	{
		retval = -1;
	}
	return retval;

}


/**
 *
 * @brief Funktion zum Aufräumen
 *
 * Ruft die Funktionen removeSemaphores() und removeSharedMemory() auf, überprüft auf Fehler
 *
 * @param  semid0 ID der 1. Semaphore
 * @param  semid1 ID der 2. Semaphore
 * @param  shmptr Pointer auf Shared Memory
 * @param  shmid ID des Shared Memory
 * 
 */
void clean(int semid0, int semid1, int *shmptr, int shmid)
{
	if(removeSemaphores(semid0, semid1) < 0){
		perror("Error removing the semaphores!");
	}
	if(removeSharedMemory(shmptr, shmid) == -1){
		exit(1);
	}
}

