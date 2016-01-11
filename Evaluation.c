#include "Affichage.h"
#include "Evaluation.h"
#include "Commandes_Internes.h"

int fd[2];
int file;

int evaluerExpr(Expression *e, t_process* process, int fdin,int fdout,int fderror) {
    pid_t pid;
    int status;
    int i;

    switch (e->type) {
    	case VIDE:
		    printf("\n");
		    break;
        case SIMPLE:
			if(internalCommands(e) != 1) {
				if(!(pid = fork())) { //child processus
					if(process!=NULL) {
						process->pid = pid;
						printf("Name : %s", process->name);
					}
					if(fdin != 0){
						dup2(fdin,0);
						close(fdin);
					}
					if(fdout != 1){
						dup2(fdout,1);
						close(fdout);
					}
					if(fderror != 2){
						dup2(fderror,2);
						close(fderror);
					}
					execvp(e->arguments[0], e->arguments);
					perror(e->arguments[0]);
					exit(1);
				} else if (pid > 0) { //father processus
					wait(NULL);					
				} else //error fork
					perror("fork");
			}
			break;
        case SEQUENCE:
		    evaluerExpr(e->gauche,NULL,fdin,fdout,fderror);
		    evaluerExpr(e->droite,NULL,fdin,fdout,fderror);
		    break;
        case SEQUENCE_ET:
		    if (evaluerExpr(e->gauche,NULL,fdin,fdout,fderror))
		    	evaluerExpr(e->droite,NULL,fdin,fdout,fderror);
		    break;
        case SEQUENCE_OU:
		    if (!evaluerExpr(e->gauche,NULL,fdin,fdout,fderror))
		    	evaluerExpr(e->droite,NULL,fdin,fdout,fderror);
		    break;
        case BG:
		    evaluerExpr(e->gauche,NULL,fdin,fdout,fderror);
		    break;
        case PIPE:
		    if(pipe(fd) < 0) {
		        perror("pipe");
		        exit(1);
        	}
		    evaluerExpr(e->gauche,NULL,fdin,fd[1],fderror);
	        evaluerExpr(e->droite,NULL,fd[0],fdout,fderror);
		    break;
        case REDIRECTION_I:
		    file = open(e->arguments[0],O_RDONLY, 0666);
		    evaluerExpr(e->gauche,NULL,file,fdout,fderror);
		    break;
        case REDIRECTION_O:
		    file = open(e->arguments[0],O_CREAT | O_RDWR, 0666);
		    evaluerExpr(e->gauche,NULL,fdin,file,fderror);
		    break;
        case REDIRECTION_A:
		    file = open(e->arguments[0], O_TRUNC | O_CREAT | O_RDWR, 0666);
		    evaluerExpr(e->gauche,NULL,fdin,file,fderror);
		    break;
        case REDIRECTION_E:
		    file = open(e->arguments[0], O_CREAT | O_RDWR, 0666);
		    evaluerExpr(e->gauche,NULL,fdin,fdout,file);
		    break;
		default:
		    break;
    }
    return 0;
}

int internalCommands(Expression *e) {
    if(strcmp(e->arguments[0],"echo") == 0)
        myEcho(e->arguments[1]);
    else if(strcmp(e->arguments[0],"date") == 0)
        myDate();
    else if(strcmp(e->arguments[0],"history") == 0)
        myHistory();
    else if(strcmp(e->arguments[0],"pwd") == 0)
        myPwd();
    else if(strcmp(e->arguments[0],"cd") == 0)
        myCd(e->arguments[1]);
    else if(strcmp(e->arguments[0],"hostname") == 0)
        myHostname();
    else if(strcmp(e->arguments[0],"kill") == 0)
        myKill(e->arguments[1]);
    else if(strcmp(e->arguments[0],"exit") == 0)
    	myExit();
    else if(strcmp(e->arguments[0],"remote") == 0) {
    	if(strcmp(e->arguments[1],"add") == 0) {
    		int i = 2;
    		for (i; e->arguments[i]!= NULL; i++)
    			processList = insertProcess(e->arguments[i]);
    	} else if(strcmp(e->arguments[1],"remove") == 0) {
    		processList = deleteProcess(e->arguments[2]);
    	} else if(strcmp(e->arguments[1],"list") == 0) {
    		printProcess();
    	} else if(strcmp(e->arguments[1],"all") == 0) {
    		t_process* process = processList;
    		Expression *e;
			e->type = SIMPLE;
			e->arguments = e->arguments+2;
    		while(process != NULL) {
				evaluerExpr(e,process,0,1,2);
				expression_free(e);			
				process = process->next;
			}
    	} else /*Specific shell*/ {
    		cmdProcess(e->arguments[1], e->arguments+2);
    	}
    }
    else return 0;
    return 1;
}



/********************************************************************
*****************            PROCESS                *****************
********************************************************************/

/*
 * Insert a new process in the list of process
 */
t_process* insertProcess(char* name) {
	t_process *newProcess = malloc(sizeof(t_process));

    newProcess->name = (char*) malloc(sizeof(name));
    newProcess->name = strcpy(newProcess->name, name);
    newProcess->next = NULL;
    
    if(processList == NULL) {
    	newProcess->id = 1;
    	return newProcess;
    }
    else {
    	int i = 1;
    	t_process* other = processList;
    	while (other->next != NULL) {
    		other = other->next;
    		i++;
    	}
    	newProcess->id = i+1;
    	other->next = newProcess;
    	return processList;
    }
}

/*
 * Delete a specific process in the list of process
 */
t_process* deleteProcess(char* name) {
	if(processList!=NULL) {
		t_process* actual = processList->next;
		t_process* before = processList;
		// If we want to delete the first process
		if(strcmp(before->name,name) == 0)
			return actual;
		
		while(actual != NULL) {
			if(strcmp(actual->name,name) == 0)
				before->next = actual->next;
			before = actual;
			actual = actual->next;
		}
	}
	return processList;
}

void printProcess() {
	printf("Processus :\n");
	printf("ID	|                Name\n");
	t_process* process = processList;
	if(process == NULL)
		printf("No one");
	else {
		while(process!=NULL) {
			printf("%d\t|%20s\n", process->id, process->name);
			process = process->next;
		}
	}
}

void cmdProcess(char* name, char* args[]) {
	t_process* process = processList;
	while(process != NULL) {
		if(strcmp(process->name, name) == 0) {
			Expression *e;
			e->type = SIMPLE;
			e->arguments = args;
			evaluerExpr(e,process,0,1,2);
			
			expression_free(e);
			break;
		}
		process = process->next;
	}	
}



