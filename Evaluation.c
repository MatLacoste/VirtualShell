#include "Affichage.h"
#include "Evaluation.h"
#include "Commandes_Internes.h"


int evaluerExpr(Expression *e, int fdin,int fdout,int fderror)
{
    pid_t pid;
    int status;
    int file;
    int fd[2];
    int i;

    switch (e->type) {
    	case VIDE:
		    printf("\n");
		    break;
        case SIMPLE:
			if(internalCommands(e) != 1) {
				if(!(pid = fork())) { //child processus
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
					execvp(e->arguments[0], &e->arguments[0]);
					perror(e->arguments[0]);
					exit(1);
				} else if (pid > 0) { //father processus
					wait(NULL);					
				} else //error fork
					perror("fork");
			}
			break;
        case SEQUENCE:
		    evaluerExpr(e->gauche,fdin,fdout,fderror);
		    evaluerExpr(e->droite,fdin,fdout,fderror);
		    break;
        case SEQUENCE_ET:
		    evaluerExpr(e->gauche,fdin,fdout,fderror);
		    evaluerExpr(e->droite,fdin,fdout,fderror);
		    break;
        case SEQUENCE_OU:
		    evaluerExpr(e->gauche,fdin,fdout,fderror);
		    evaluerExpr(e->droite,fdin,fdout,fderror);
		    break;
        case BG:
		    evaluerExpr(e->gauche,fdin,fdout,fderror);
		    break;
        case PIPE:
		    if(pipe(fd) < 0) {
		        perror("pipe");
		        exit(1);
        	}
		    evaluerExpr(e->gauche,fdin,fd[1],fderror);
	        evaluerExpr(e->droite,fd[0],fdout,fderror);
		    break;
        case REDIRECTION_I:
		    file = open(e->arguments[0],O_RDONLY, 0666);
		    evaluerExpr(e->gauche,file,fdout,fderror);
		    break;
        case REDIRECTION_O:
		    file = open(e->arguments[0],O_CREAT | O_RDWR, 0666);
		    evaluerExpr(e->gauche,fdin,file,fderror);
		    break;
        case REDIRECTION_A:
		    file = open(e->arguments[0], O_TRUNC | O_CREAT | O_RDWR, 0666);
		    evaluerExpr(e->gauche,fdin,file,fderror);
		    break;
        case REDIRECTION_E:
		    file = open(e->arguments[0], O_CREAT | O_RDWR, 0666);
		    evaluerExpr(e->gauche,fdin,fdout,file);
		    break;
        case REDIRECTION_EO:
		    file = open(e->arguments[0], O_CREAT | O_RDWR, 0666);
		    evaluerExpr(e->gauche,fdin,file,file);
		    break;
		default:
		    break;
    }
    return 0;
}

int internalCommands(Expression *e) {
    
    if(strcmp(e->arguments[0],"echo") == 0) {
        myEcho(e->arguments[1]);
        return 1;
    } else if(strcmp(e->arguments[0],"date") == 0) {
        myDate();
        return 1;
    } else if(strcmp(e->arguments[0],"history") ==0){
        myHistory();
        return 1;
    } else if(strcmp(e->arguments[0],"pwd") == 0) {
        myPwd();
        return 1;
    } else if(strcmp(e->arguments[0],"cd") == 0) {
        myCd(e->arguments[1]);
        return 1;
    } else if(strcmp(e->arguments[0],"hostname") == 0) {
        myHostname();
        return 1;
    } else if(strcmp(e->arguments[0],"kill") == 0) {
        myKill(e->arguments[1]);
        return 1;
    } else if(strcmp(e->arguments[0],"exit") == 0) {
    	myExit();
        return 1;
    } else return 0;
}
