#ifndef COMINTERN_H
#define COMINTERN_H
#include "Shell.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>

#define NB_COMMAND_MAX 50

char COMMAND_LIST[NB_COMMAND_MAX][1024];

void  myEcho (char *);
void  myDate ();
void  myHistory ();
void  addCommand (char *);
char* myPwd ();
void  myCd (char *);
char* myHostname ();
void  myKill(char *);
void  myExit();

#endif
