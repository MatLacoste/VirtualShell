#include "Commandes_Internes.h"

void myEcho (char *sentence) {
	printf ("%s\n", sentence);
}

void myDate () {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int hours = tm.tm_gmtoff/3600;
	int min = tm.tm_gmtoff - (hours*3600);
	char positif;

	if (hours > 0) { 
		positif = '+';
	} else {
		positif = '-';
		hours *= -1;
		min *= -1;
	}

	printf("%s %d %s %d, %02d:%02d:%02d (UTC%c%02d%02d)\n", JOURS[tm.tm_wday], tm.tm_mday, MOIS[tm.tm_mon], tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, positif, hours, min);	
}

void myHistory () {
	int i;
	for (i=0; i<NB_COMMAND; i++)
			printf ("%d  %s\n", (i+1), COMMAND_LIST[i]);
}

void addCommand (char* newCommand) {
	int i;
	if (NB_COMMAND == NB_COMMAND_MAX) {
		for (i=0; i<NB_COMMAND_MAX-1; i++) {
			 strcpy(COMMAND_LIST[i], COMMAND_LIST[i+1]);
		}
		strcpy(COMMAND_LIST[NB_COMMAND_MAX-1], newCommand);
	} else {
		strcpy(COMMAND_LIST[NB_COMMAND], newCommand);
		NB_COMMAND++;
	}
}

char* myPwd () {
	char cwd[1024];
	getcwd (cwd, sizeof(cwd));
	return cwd;
}

void myCd (char* name) {
	char* pwd = myPwd ();
	strcat (pwd, "/");
	strcat (pwd, name);
	if (chdir (pwd) == -1)
		printf ("Le dossier %s n'existe pas\n", name);
	else
		printf ("Vous êtes entré dans le dossier %s\n", name);
}

char* myHostname () {
	char hostname[1024];
	gethostname (hostname, sizeof (hostname));
	return hostname;
}

