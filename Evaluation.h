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

typedef struct process {
	int id;
	char* name;
	pid_t pid;
	struct t_process* next;
} t_process;

static t_process* processList = NULL;

extern int evaluerExpr(Expression *, t_process* ,int, int, int);
extern int internalCommands(Expression *);

t_process* insertProcess(char*);
t_process* deleteProcess(char*);
void printProcess();

void cmdProcess(char*, char*[]);

#endif
