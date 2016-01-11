#ifndef COMINTERN_H
#define COMINTERN_H
#include "Shell.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>

void  myEcho (char *);
void  myDate ();
void  myHistory();
char* myPwd ();
void  myCd (char *);
char* myHostname ();
void  myKill(char *);
void  myExit();

#endif
