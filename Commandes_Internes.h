#ifndef COMINTERN_H
#define COMINTERN_H
#include "Shell.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>

#define NB_COMMAND_MAX 50

int NB_COMMAND = 0;
char COMMAND_LIST[NB_COMMAND_MAX][1024];


	char JOURS[7][10] = {"dimanche", "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi"};
	char MOIS[12][10] = {"janvier", "février", "mars", "avril", "mai", "juin", "juillet", "août", "septembre", "novembre", "decembre"};

void  myEcho (char *);
void  myDate ();
void  initHistory();
void  myHistory ();
void  addCommand (char *);
char* myPwd ();
void  myCd (char *);
char* myHostname ();
void  myKill(char *);
void  myExit();

#endif
