#ifndef _EVALUATION_H
#define _EVALUATION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

extern int evaluerExpr(Expression *, int, int, int);
extern int internalCommands(Expression *);

#endif
