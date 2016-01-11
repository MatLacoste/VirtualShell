#include "Commandes_Internes.h"



int nbCommand = 0;

void myEcho (char *sentence) {
	printf ("%s\n", sentence);
}

void myDate () {
	char JOURS[7][10] = {"dimanche", "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi"};
	char MOIS[12][10] = {"janvier", "février", "mars", "avril", "mai", "juin", "juillet", "août", "septembre", "novembre", "decembre"};

	time_t t = time(NULL);
	time_t gmt = 0;
	time_t offset = mktime(gmtime(&gmt));
	struct tm tm = *localtime(&t);

	int hours = offset/3600;
	int min = (offset-(hours*3600))/60;
	char positif;

	if (hours > 0) { 
		positif = '-';
	} else {
		positif = '+';
		hours *= -1;
		min *= -1;
	}

	printf("%s %d %s %d, %02d:%02d:%02d (UTC%c%02d%02d)\n", JOURS[tm.tm_wday], tm.tm_mday, MOIS[tm.tm_mon], tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, positif, hours, min);	
}

void myHistory () {
	HIST_ENTRY **the_history_list =  history_list ();
    int i;
    for(i = 0; the_history_list[i] != NULL; i++) {
		printf("%d  %s\n", i+1, the_history_list[i]->line);
    }
}

char* myPwd () {
	char cwd[1024];
	getcwd (cwd, sizeof(cwd));
	return cwd;
}

void myCd (char* name) {
	if (name != NULL)
		chdir(name);
	else chdir(getenv("HOME"));
}

char* myHostname () {
	char hostname[1024];
	gethostname (hostname, sizeof (hostname));
	return hostname;
}

void myKill(char *pid) {
	if(pid != NULL)
    	kill(pid, SIGKILL);
}

void myExit() {
	exit(EXIT_SUCCESS);
}
