#include "Commandes_Internes.h"

char* myPwd () {
	char cwd[1024];
	getcwd (cwd, sizeof(cwd));
	return cwd;
}

void myCd (char* name) {
	char* pwd = myPwd ();
	strcat (pwd, "/");
	strcat (pwd, name);
	if (chdir (pwd) == -1)
		printf ("Le dossier %s n'existe pas\n", name);
	else
		printf ("Vous êtes entré dans le dossier %s\n", name);
}

char* myHostname () {
	char hostname[1024];
	gethostname (hostname, sizeof (hostname));
	return hostname;
}

