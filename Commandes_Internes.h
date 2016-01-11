#ifndef COMINTERN_H
#define COMINTERN_H
#include "Shell.h"
#include <unistd.h>
#include <string.h>

#define NB_COMMAND_MAX 50

char COMMAND_LIST[NB_COMMAND_MAX][1024];
int NB_COMMAND=0;
char JOURS[7][10] = {"dimanche", "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi"};
char MOIS[12][10] = {"janvier", "février", "mars", "avril", "mai", "juin", "juillet", "août", "septembre", "novembre", "decembre"};

void myEcho (char *sentence);
void myDate ();
void myHistory ();
void addCommand (char* newCommand);
char* myPwd ();
void myCd (char* name);

#endif
