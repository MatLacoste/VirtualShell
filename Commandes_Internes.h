#ifndef COMINTERN_H
#define COMINTERN_H
#include "Shell.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>

int myEcho (char *);
int myDate ();
int myHistory();
int myPwd ();
int myCd (char *);
int myHostname ();
int myKill(char *);
int myExit();

#endif
