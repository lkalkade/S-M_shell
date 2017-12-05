/*
 ============================================================================
 Nom        	      : shell_KALKA-DEBIDINE_Ludovic.c
 Auteur      	      : Ludovic KALKA-DEBIDINE
 Date de création     : 02/05/2015
 Date de modification : 14/05/2015
 Version     	      : v4.5
 Description 	      : Simple Shell en C
 ============================================================================
 */

/* en-têtes standard */
#include <sys/types.h>	/* pour pid_t */
#include <stdlib.h>	/* pour exit() */
#include <stdio.h>	/* pour fprintf()*/
#include <unistd.h>	/* pour fork() et pipe() */

#include <sys/wait.h>   /* pour avoir wait & co. */
#include <ctype.h>      /* pour avoir isspace & co. */
#include <errno.h>      /* pour avoir errno */
#include <signal.h>     /* pour attraper le SIGINT */
#include <fcntl.h> 	/* pour creat() */
#include <sys/types.h>	/* pour pid_t */
#include <string.h>

/*Couleur texte dans le shell*/
#define NORMAL  "\x1B[0m"

#define ROUGE  "\x1B[31m"
#define VERT  "\x1B[32m"
#define JAUNE  "\x1B[33m"
#define BLEU  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define BLANC  "\x1B[37m"


#define MAXLINE 4096
#define MAXELEMS 128


/*** Variables Globales
* 
*/
char ligne[MAXLINE]; // tableau qui contient les commandes entrées par l'utilisateur
char *commandes[MAXELEMS]; // pointeur sur les mots d'une chaine
char *arg[MAXELEMS];	
int nb_commandes;		// Taille du tableau "commandes"
int nb_arg;	// Taille du tableau "arg"
 
int front_pid = -1;

/*** Fonction afficheinviteCommande
* Fonction qui permet d'afficher un signe d'invite sympathique.
*/
void afficheinviteCommande()
{
	char pwd[500];	
	
	/*pour avoir le repertoire courant */	
	if(getcwd(pwd, 500) == NULL) 
	{
	  	printf ("impossible d'avor le repertoire courant\n");
	}
	
	else 
	{
  		printf("%sSHELL%s%s@%s%sKALKA%s:~%s$ ",BLEU, NORMAL, JAUNE, NORMAL, VERT, NORMAL, pwd);
		fflush(stdin);
	}
}

/*** Fonction affiche_invite
* Fonction qui permet d'afficher un signe d'invite sympathique.
*/
int affiche_invite(char chaine[10])
{
	FILE *f;
	FILE *f1;
    char s1[500];
    char s[500];
    
    if(strcmp(chaine,"-r")==0){
	getcwd(s,500);
	
   printf("%s:~$ ", s);
    
	}
	else if( strcmp(chaine,"-h")==0)
	{/* j'ouvre la commande 'hostname'en lecture*/
		f = popen("hostname", "r"); 
		fscanf(f, "%s", s);/*je met la valeur de 'hostname' dans la chaine s*/
		
		printf("%s:~$ ", s);/*je l'affiche en invite de commande*/
		pclose(f);/* je ferme*//* ce que je fais la c'est exactement la même chose pour tout le reste des invites*/
	}
	else if( strcmp(chaine,"-d")==0)
	{
		f = popen("date", "r"); 
		fscanf(f, "%s", s);
		printf("%s:~$ ", s);
		pclose(f);
	}
	else if(strcmp(chaine,"-u")==0)
	{
			f = popen("users", "r"); 
			fscanf(f, "%s", s);
			printf("%s:~$ ", s);
			pclose(f);
	}
	else if(strcmp(chaine,"-rh")==0)
	{
			
			f = popen("hostname", "r"); 
			fscanf(f, "%s", s1);
			getcwd(s,500);
			printf(" %s:%s~$ ", s,s1);
			pclose(f);
			
	}
	else if(strcmp(chaine,"-hr")==0)
	{
			
			f = popen("hostname", "r"); 
			fscanf(f, "%s", s1);
			getcwd(s,500);
			printf(" %s:%s~$ ", s1,s);
			pclose(f);
			
	}
	else if(strcmp(chaine,"-hd")==0)
	{
			
			f1 = popen("hostname", "r"); 
			fscanf(f1, "%s", s1);
			pclose(f1);
			f = popen("date", "r"); 
			
			fscanf(f, "%s", s);
			printf(" %s:%s~$ ", s1,s);
			pclose(f);
			
	}
	else if(strcmp(chaine,"-dh")==0)
	{
			
			f1 = popen("hostname", "r"); 
			fscanf(f1, "%s", s1);
			pclose(f1);
			f = popen("date", "r"); 
			fscanf(f, "%s", s);
			
			printf(" %s:%s~$ ", s,s1);
			
			pclose(f);
			
	}
	else if(strcmp(chaine,"-ht")==0)
	{
			
			f1 = popen("hostname", "r"); 
			fscanf(f1, "%s", s1);
			pclose(f1);
			f = popen("date +%T", "r"); 
			
			fscanf(f, "%s", s);
			printf(" %s:%s~$ ", s1,s);
			pclose(f);
			
	}
	else if(strcmp(chaine,"-th")==0)
	{
			
			f1 = popen("hostname", "r"); 
			fscanf(f1, "%s", s1);
			pclose(f1);
			f = popen("date +%T", "r"); 
			
			fscanf(f, "%s", s);
			printf(" %s:%s~$ ", s,s1);
			pclose(f);
			
	}
	else if(strcmp(chaine,"-td")==0)
	{
			
			f1 = popen("date", "r"); 
			fscanf(f1, "%s", s1);
			pclose(f1);
			f = popen("date +%T", "r"); 
			
			fscanf(f, "%s", s);
			printf(" %s:%s~$ ", s,s1);
			pclose(f);
			
	}
	else if(strcmp(chaine,"-dt")==0)
	{
			
			f1 = popen("date", "r"); 
			fscanf(f1, "%s", s1);
			pclose(f1);
			f = popen("date +%T", "r"); 
			
			fscanf(f, "%s", s);
			printf(" %s:%s~$ ", s1,s);
			pclose(f);
			
	}
	else if(strcmp(chaine,"-t")==0)
	{
			f = popen("date +%T", "r"); 		
			fscanf(f, "%s", s);
			printf(" %s:~$ ",s);
			pclose(f);
			
	}
	
	else{/* si on comprend pas l'argument on met le repertoire courant comme invite*/
			afficheinviteCommande();
		}
	
}

/*** Fonction DecoupeligneCommande 
* Fonction qui découpe ligne en mots fait pointer chaque commandes[i] sur un mot différent commandes se termine par NULL
*/
void decoupeligneCommande()
{
  char* debut = ligne;
  int i;
  for (i=0; i<MAXELEMS-1; i++) {

    /* saute les espaces */
    while (*debut && isspace(*debut)) debut++;

    /* fin de ligne ? */
    if (!*debut) break;

    /* on se souvient du début de ce mot */
    commandes[i] = debut;

    /* cherche la fin du mot */
    while (*debut && !isspace(*debut)) debut++; /* saute le mot */

    /* termine le mot par un \0 et passe au suivant */
    if (*debut) { *debut = 0; debut++; }
  }

  commandes[i] = NULL;
}


/*** Fonction LectureLigneCommande
* Fonction qui permet de lire une ligne en entrée standard.  
*/
void lectureLigneCommande()
{
  //fgets lit une ligneCommande en entrée standard (stdin), puis la copie dans le buffer passé en argument (ligne) jusqu'à (MAXLINE-1)
	
	if (fgets(ligne, MAXLINE, stdin) == NULL)
	{
		fflush(stdin);
	}
	char separateur[] = "\n";
	int nb_commandes = 0;

	commandes[nb_commandes] = strtok(ligne, separateur);

	while (commandes[nb_commandes] != NULL)
	{
		++nb_commandes;
		commandes[nb_commandes] = strtok(NULL, separateur);
	}
}

/*** Fonction AttendreFils
* Fonction qui permet d'attendre la fin d'execution d'un processus fils.
*/
void attendreFils()
{
/* un signal peut correspondre à plusieurs fils finis, donc on boucle */
  while (1) {
    int status;
    pid_t pid = waitpid(-1,&status,WNOHANG);
    if (pid<0) { 
      if (errno==EINTR) continue; /* interrompu => on recommence */
      if (errno==ECHILD) break;   /* plus de fils terminé => on quitte */
      printf("erreur de wait (%s)\n",strerror(errno));
      break;
    }

    if (pid==0) break; /* plus de fils terminé => on quitte */

    /*signale à execute que front_pid s'est terminé */
    if (pid==front_pid) front_pid = -1;

    if (WIFEXITED(status))
      printf("terminaison normale, pid=%i, status %i\n",pid,WEXITSTATUS(status));
    if (WIFSIGNALED(status))
      printf("terminaison par signal %i, pid=%i\n",WTERMSIG(status),pid);
  }
}

/*** Fonction ExecuterligneCommande
 * Fonction qui permet d'executer une ligne de commande.
 */
void executeLigneCommande()
{
  sigset_t sigset;
  pid_t pid;
  int en_fond;
  int i;	

  /*détection du & */
  if (strchr(ligne,'&')) {
    *strchr(ligne,'&') = 0;
    en_fond = 1;
  }
  else {
    /* bloque SIGCHLD jusqu'à ce que le père ait placé le pid du
       fils dans front_pid
       sinon on risque de manquer la notification de fin du fils 
       (race condition)
    */
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGCHLD);
    sigprocmask(SIG_BLOCK,&sigset,NULL);
    en_fond = 0;
  }
   	if(strchr(ligne,'|'))
	{
		cmd_pipe(ligne);		
		return;
	}
	 else if(strchr(ligne,'>'))
	{
		redirigeSortie(ligne);		
		return;
	}
     	else if(strchr(ligne,'<'))
	{
		redirigeEntree(ligne);		
		return;
	}
	else if(strchr(ligne,';'))
	{
		double_cmd(ligne);		
		return;
	}
  decoupeligneCommande();
  if (!commandes[0]) return; /* ligne vide */
  else if(built_in(ligne))/* c'est une fonction que j'ai programmé et je l'appel builint*/
			{
			built_in(ligne);
			return;
			}
  /*cherche le dernier argument de la ligne */
  pid = fork();
  if (pid < 0) {
    printf("fork a échoué (%s)\n",strerror(errno));
    return;
  }

  if (pid==0) {
    /* fils */	
    if (en_fond) {
      /*redirection de l'entrée standard sur /dev/null */
	 //printf("\n[%i] %s\n",getpid(),ligne);
      int devnull = open("/dev/null",O_RDONLY);
      if (devnull != -1) {
	close(0);
	dup2(devnull,0);
      }
    }
    else {
      /*réactivation de SIGINT & débloque SIGCLHD */
      struct sigaction sig;
      sig.sa_flags = 0;
      sig.sa_handler = SIG_DFL;
      sigemptyset(&sig.sa_mask);
      sigaction(SIGINT,&sig,NULL);
      sigprocmask(SIG_UNBLOCK,&sigset,NULL);
    }
    
    execvp(commandes[0], /* programme à exécuter */
	   commandes     /* argv du programme à exécuter */
	   );
    printf("impossible d'éxecuter \"%s\" (%s)\n",commandes[0],strerror(errno));
    exit(1);
  }

  else {

    if (!en_fond) {

      /*attent la fin du processus au premier plan */
      printf("pid %i\n",pid);
      front_pid = pid;
      sigprocmask(SIG_UNBLOCK,&sigset,NULL);

      /* attente bloquante jusqu'à ce que sigchld signale que front_pid
         s'est terminé */
      while (front_pid!=-1) pause();
    }

  }
}

/*** Fonction redirigeSortie
* Fonction qui permet de rediriger la sortie standard de "arg1 vers arg2" comme "ls > fic".
*/
int redirigeSortie(char* bidul)
{		int i = 0;
    int sorti = 0;
	
        bidul = strtok(ligne, " \t\n");
        // regarde tout auto de la cmd
        while (bidul != NULL)
        {
            if (strcoll(bidul, ">") == 0)
            { 
                sorti = i;
            }
            arg[i++] = bidul;
            bidul = strtok(NULL, " \t\n");
        }
        arg[i] = 0;
        if (sorti > 0)
       {
            char *gauchecmnd[sorti+1];
            char *sorticmnd[i-sorti];
            int a, b;
         
			
        for (b = 0; b < sorti; b++)
                gauchecmnd[b] = arg[b];

            gauchecmnd[sorti] = NULL;

            for (a = 0; a < i-sorti-1; a++)
                sorticmnd[a] = arg[a+sorti+1];

            sorticmnd[i-sorti-1] = NULL; 
		if(!fork())/*on ouvre la sorti en ecriture*/
		{	 if ((freopen(sorticmnd[0],"w", stdout)) != NULL)
				{
					/* Exécuter le programme */
				execvp(gauchecmnd[0], gauchecmnd);
					fclose (stdout);
				}
				
			
		}
	
	else{	

				wait(NULL);
		}
	}

}


/*** Fonction redirigeEntree
* Fonction qui permet de rediriger l'entrée standard. Cette forction permet notamment de simuler "less < fic".
*/
int redirigeEntree(char* bidule)
{		int i = 0;
    int sorti = 0;
	
        bidule = strtok(ligne, " \t\n");
        // regarde tout auto de la cmd
        while (bidule != NULL)
        {
            if (strcoll(bidule, "<") == 0)
            { 
                sorti = i;
            }
            arg[i++] = bidule;
            bidule = strtok(NULL, " \t\n");
        }
        arg[i] = 0;
        if (sorti > 0)
       {
            char *gauchecmnd[sorti+1];
            char *entreecmnd[i-sorti];
            int a, b;
         
			
        for (b = 0; b < sorti; b++)
                gauchecmnd[b] = arg[b];

            gauchecmnd[sorti] = NULL;

            for (a = 0; a < i-sorti-1; a++)
                entreecmnd[a] = arg[a+sorti+1];

            entreecmnd[i-sorti-1] = NULL; 
		if(!fork())/*on ouvre la sorti en ecriture*/
		{	 if ((freopen(entreecmnd[0],"r", stdin)) != NULL)
				{
					/* Exécuter le programme */
				execvp(gauchecmnd[0], gauchecmnd);
					fclose (stdin);
				}
				
			
		}
	
	else{	

				wait(NULL);
		}
	}

}

/*** Fonction double_cmd
* Fonction qui permet d'executer une double commande. Cette fonction permet notamment de simuler "ls -l ; ls".
*/
int double_cmd( char *token)
{	
		int i = 0;
		int j=0;
    int indictr = 0;
		
        token = strtok(ligne, " \t\n");
       
        while (token != NULL)
        {
            if (strcoll(token, ";") == 0)
            { 
                indictr = i;
       
            }
            arg[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        arg[i] = 0;

        // et on fait la repartition des deux parites de la cmd.
        if (indictr > 0)
        {
            char *leftcmnd[indictr+1];
            char *rightcmnd[i-indictr];
            int a, b;

            for (b = 0; b < indictr; b++){
                leftcmnd[b] = arg[b];}

            leftcmnd[indictr] = NULL;

            for (a = 0; a < i-indictr-1; a++){
                rightcmnd[a] = arg[a+indictr+1];
                }

				rightcmnd[i-indictr-1] = NULL;  // Did not include -1
	
            	if(!fork())
		{
			execvp(leftcmnd[0], leftcmnd);
		}
		if (!fork())
            	{	
			execvp(rightcmnd[0],rightcmnd);					
						
		}
		else
		{	
			wait(NULL);
			wait(NULL);
		}
	}
return 0;
}

/*** Fonction cmd_pipe
 * Fonction qui permet d'executer un commande avec un pipe "|".
 */
int cmd_pipe( char *tkn)
{		int i = 0;
    int indictr = 0;
	
        tkn = strtok(ligne, " \t\n");
        // regarde tout auto de la cmd
        while (tkn != NULL)
        {
            if (strcmp(tkn, "|") == 0)
            { 
                indictr = i;
            }
            arg[i++] = tkn;
            tkn = strtok(NULL, " \t\n");
        }
        arg[i] = 0;

        // et on fait la repartition des deux parites de la cmd.
        if (indictr > 0)
        {
            char *leftcmnd[indictr+1];
            char *rightcmnd[i-indictr];
            int a, b;

            for (b = 0; b < indictr; b++){
                leftcmnd[b] = arg[b];}

            leftcmnd[indictr] = NULL;

            for (a = 0; a < i-indictr-1; a++){
                rightcmnd[a] = arg[a+indictr+1];}

            rightcmnd[i-indictr-1] = NULL;  // Did not include -1

            if (!fork())
            {
                fflush(stdin);
                int fd[2];
                pipe(fd);

                if (!fork())
                {
                    /*la partie gauche de la commande avec le pipe*/
                    dup2(fd[1], STDOUT_FILENO);
		    close(fd[0]);
                    execvp(leftcmnd[0], leftcmnd);
                    fprintf(stderr, "impossible d'executer  %s\n", leftcmnd[0]);
                    exit(1);
                }
                else
                {
					/*la partie droite de la commande avec le pipe*/
                    fflush(stdin);
                    dup2(fd[0], STDIN_FILENO);
		    close(fd[1]);
                    execvp(rightcmnd[0], rightcmnd);
                    fprintf(stderr, "impossible d'executer %s\n", rightcmnd[0]);
                    exit(1);
                }
            }
            else
                wait(NULL);
}
return 0;
}

/*** Fonction Built_in
 * Fonction qui permet d'executer un built-int.
 */
int built_in(char cmd[20])
{
	int i;
	//Si la commande est exit, on sort
	if (strcmp(cmd, "exit") == 0) 
	{
		exit(0);
	}
	char *home = getenv("HOME");
	//Si la commande est cd on regarde si le mot suivant est nul ou si on peut aller à l'endroit spécifié
	if (strcmp(cmd, "cd") == 0) 
	{
		if (!cmd[1]) //si nul go home
		{
		
			if (chdir(home) == -1) 
			{
				fprintf(stderr, "Impossible d'accéder au dossier désiré.\n");
				exit(EXIT_FAILURE);
			}
		}
    
		else 
		{
			if (strcmp(cmd + 3, "home") != 0)	// souschaine 1 different de home
			{
				if (chdir(cmd + 3) == 0) 
				{
					chdir(cmd + 3);
					return 1; 
				}
			}
      
			else 
			{
				//Si home go home
				if (chdir(home) == -1) 
				{
					fprintf(stderr, "Impossible d'accéder au dossier désiré.\n");
					exit(EXIT_FAILURE);
				}
			}
		}
		return 1; 
	}
	return 0;  

}

/*Main*/
int main(int argc, char *argv[])
{
  struct sigaction sig;
  sig.sa_flags = 0;
  sig.sa_handler = attendreFils;
  sigemptyset(&sig.sa_mask);
  sigaction(SIGCHLD,&sig,NULL);
  sig.sa_handler = SIG_IGN;
  
  if(argc > 1)/* si le programme a reçu d'argument*/
  {
	while(1)
	{
		affiche_invite(argv[1]);
		lectureLigneCommande();
		executeLigneCommande();
	}
  }

  else/* s'il y a pas d'argument*/
  {		
	while(1)
	{
		afficheinviteCommande();
		lectureLigneCommande();
		executeLigneCommande();
	}
  }
  return 0;
}
