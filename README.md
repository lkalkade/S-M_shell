# S-M_shell
Shell à moi en C

Programme réalisé en 2ème année de licence

## Fonctionnement du programme.
Une invite de commande apparait alors et vous pouvez saisir des commandes telles que :

      * ls (options) //Nous avons testé avec ls -la notamment
      * cd (options) //cd et cd home renvoient au "HOME" (Built-in)
      * man (options)
      * pwd
      * cp (source destination) //cp fichier.c .. par exemple
      * rm (fichier) //rm ../fichier.c par exemple
      * mkdir (nom de dossier)
      * echo (entrée au clavier)
      * ps -ef
      * exit (Built-in)
      
etc.
il est possible de rediriger les entrées et sorties.
permet ainsi les redirections telles que :

      * ls > monFichier
      * ls (options) > monFichier
      * less < monFichier

Il est possible d'entrer au clavier des commandes telles que :

      * ps -ef | more -f
      * ls | wc
      * echo bonjour | wc
      * ls ; ls -l
      
Il est possible de bloquer le signal ctrl-c et de demeurer à l'intérieur du programme lorsque celui-ci
est envoyé.
Il est possible de lancer des programmes en arrière plan.
