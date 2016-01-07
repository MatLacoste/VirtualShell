/* Construction des arbres repr�sentant des commandes */

#include "Shell.h"
#include "Affichage.h"
#include "Evaluation.h"

#include <stdbool.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

extern int yyparse_string(char *);

 bool interactive_mode = 1; // par d�faut on utilise readline 
 int status = 0; // valeur retourn�e par la derni�re commande


/*
 * Construit une expression � partir de sous-expressions
 */
Expression *ConstruireNoeud (expr_t type, Expression *g, Expression *d, char **args)
{
  Expression *e;
     
  if ((e = (Expression *)malloc(sizeof(Expression))) == NULL){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  e->type   = type;
  e->gauche = g;
  e->droite = d;
  e->arguments = args;
  return e;
} /* ConstruireNoeud */



/*
 * Renvoie la longueur d'une liste d'arguments
 */
int LongueurListe(char **l)
{
  char **p;
  for (p=l; *p != NULL; p++)
    ;
  return p-l;
} /* LongueurListe */



/*
 * Renvoie une liste d'arguments, la premi�re case �tant initialis�e � NULL, la
 * liste pouvant contenir NB_ARGS arguments (plus le pointeur NULL de fin de
 * liste)
 */
char **InitialiserListeArguments (void)
{
  char **l;
  
  l = (char **) (calloc (NB_ARGS+1, sizeof (char *)));
  *l = NULL;
  return l;
} /* InitialiserListeArguments */



/*
 * Ajoute en fin de liste le nouvel argument et renvoie la liste r�sultante
 */
char **AjouterArg (char **Liste, char *Arg)
{
  char **l;
  
  l = Liste + LongueurListe (Liste);
  *l = (char *) (malloc (1+strlen (Arg)));
  strcpy (*l++, Arg);
  *l = NULL;
  return Liste;
} /* AjouterArg */



/*
 * Fonction appel�e lorsque l'utilisateur tape "".
 */
void EndOfFile (void)
{
  exit (0);
} /* EndOfFile */

/*
 * Appel�e par yyparse() sur erreur syntaxique
 */
void yyerror (char *s)
{
   fprintf(stderr, "%s\n", s);
}


/*
 * Lib�ration de la m�moire occup�e par une expression
 */
void
expression_free(Expression *e)
{
  if (e == NULL)
    return;
    
  expression_free(e->gauche);
  expression_free(e->droite);

  if (e->arguments != NULL)
    {
      for (int i = 0; e->arguments[i] != NULL; i++)
	free(e->arguments[i]);
      free(e->arguments);  
    }

  free(e);
}


/*
 * Lecture de la ligne de commande � l'aide de readline en mode interactif
 * M�morisation dans l'historique des commandes
 * Analyse de la ligne lue 
 */

int
my_yyparse(void)
{
  if (interactive_mode)
    {
      char *line = NULL;
      char buffer[1024];
      snprintf(buffer, 1024, "mini_shell(%d):", status);
      line = readline(buffer);
      if(line != NULL)
	{
	  int ret;
	  add_history(line);              // Enregistre la line non vide dans l'historique courant
	  *strchr(line, '\0') = '\n';      // Ajoute \n � la line pour qu'elle puisse etre trait� par le parseur
	  ret = yyparse_string(line);     // Remplace l'entr�e standard de yyparse par s
	  free(line);
	  return ret;
	}
      else
	{
	  EndOfFile();
	  return -1;
	}
    }
  else
    {
      // pour le mode distant par exemple
      
      int ret; int c;
          
      char *line = NULL;
      size_t linecap = 0;
      ssize_t linelen;
      linelen = getline(&line, &linecap, stdin);

      if(linelen>0)
	{
	  int ret;
	  ret = yyparse_string(line);  
	  free(line);
	  return ret;
	}    
    }
}



      /*--------------------------------------------------------------------------------------.
      | Lorsque l'analyse de la ligne de commande est effectu�e sans erreur. La variable      |
      | globale ExpressionAnalysee pointe sur un arbre repr�sentant l'expression.  Le type    |
      |       "Expression" de l'arbre est d�crit dans le fichier Shell.h. Il contient 4       |
      |       champs. Si e est du type Expression :					      |
      | 										      |
      | - e.type est un type d'expression, contenant une valeur d�finie par �num�ration dans  |
      |   Shell.h. Cette valeur peut �tre :					      	      |
      | 										      |
      |   - VIDE, commande vide								      |
      |   - SIMPLE, commande simple et ses arguments					      |
      |   - SEQUENCE, s�quence (;) d'instructions					      |
      |   - SEQUENCE_ET, s�quence conditionnelle (&&) d'instructions			      |
      |   - SEQUENCE_OU, s�quence conditionnelle (||) d'instructions			      |
      |   - BG, t�che en arri�re plan (&)						      |
      |   - PIPE, pipe (|).								      |
      |   - REDIRECTION_I, redirection de l'entr�e (<)					      |
      |   - REDIRECTION_O, redirection de la sortie (>)					      |
      |   - REDIRECTION_A, redirection de la sortie en mode APPEND (>>).		      |
      |   - REDIRECTION_E, redirection de la sortie erreur,  	   			      |
      |   - REDIRECTION_EO, redirection des sorties erreur et standard.			      |
      | 										      |
      | - e.gauche et e.droite, de type Expression *, repr�sentent une sous-expression gauche |
      |       et une sous-expression droite. Ces deux champs ne sont pas utilis�s pour les    |
      |       types VIDE et SIMPLE. Pour les expressions r�clamant deux sous-expressions      |
      |       (SEQUENCE, SEQUENCE_ET, SEQUENCE_OU, et PIPE) ces deux champs sont utilis�s     |
      |       simultann�ment.  Pour les autres champs, seule l'expression gauche est	      |
      |       utilis�e.									      |
      | 										      |
      | - e.arguments, de type char **, a deux interpretations :			      |
      | 										      |
      |      - si le type de la commande est simple, e.arguments pointe sur un tableau � la   |
      |       argv. (e.arguments)[0] est le nom de la commande, (e.arguments)[1] est le	      |
      |       premier argument, etc.							      |
      | 										      |
      |      - si le type de la commande est une redirection, (e.arguments)[0] est le nom du  |
      |       fichier vers lequel on redirige.						      |
      `--------------------------------------------------------------------------------------*/


int
main (int argc, char **argv) 
{

  // faire en sorte qu'interactive_mode = 0 lorsque le shell est distant 
  
  if (interactive_mode)
    {
      using_history();
    }
  else
    {
      //  mode distant 
    }
  
  while (1){
    if (my_yyparse () == 0) {  /* L'analyse a abouti */   
      afficher_expr(ExpressionAnalysee);
      fflush(stdout);
      expression_free(ExpressionAnalysee);
    }
    else {
      /* L'analyse de la ligne de commande a donn� une erreur */
    }
  }
  return 0;
}
