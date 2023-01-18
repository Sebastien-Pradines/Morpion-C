#include <stdbool.h>
#include <stdio.h>
#define KRED "\x1B[31m"
#define KBLU "\x1B[34m"
#define KGRN "\x1B[32m"
#define KNRM "\x1B[0m"
#define KMAG "\x1B[35m"
#include <stdio.h> 
#include <netdb.h> 
#include <fcntl.h> 
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h> // gestion adresses i 
#include <sys/types.h> 
 
#define SERV "127.0.0.1"     // adresse IP = boucle locale 
#define PORT 1234   // port d’ecoute serveur 
int port,sock, idClient;     // n°port et socket 
char mess[80];     // chaine de caracteres 
char mess2[80]; 
      
struct  sockaddr_in     serv_addr; // zone adresse 
struct  hostent         *server; // nom serveur 


creer_socket() 
{  
    port = PORT; 
    server = gethostbyname(SERV);  // verification existance adresse 
    if (!server){
        fprintf(stderr, "Problème serveur \"%s\"\n", SERV);exit(1);
    } 
    // creation socket locale 
    sock = socket(AF_INET, SOCK_STREAM, 0);     // creation socket  
    bzero(&serv_addr, sizeof(serv_addr));   // preparation champs entete   
    serv_addr.sin_family = AF_INET;   // Type d’adresses 
    bcopy(server->h_addr, &serv_addr.sin_addr.s_addr,server->h_length); 
    serv_addr.sin_port = htons(port);   // port de connexion du serveur 
} 

int l = 0;
int c = 0;
int nbJeu = 0;
int tab[3][3];

void creationTableau() {
  for (l = 0; l < 3; l++) {
    for (c = 0; c < 3; c++) {
      tab[l][c] = 9;
    }
  }
}

void affichageTableau() {
  l = 0;
  c = 0;

  printf("%s  1 2 3 \n", KNRM);
  for (l = 0; l < 3; l++) {
    printf("%d", l + 1);
    for (c = 0; c < 3; c++) {
      printf("%s|", KNRM);
      if (tab[l][c] == 1) {
        printf("%sX", KRED);
      } else if (tab[l][c] == 2) {
        printf("%sO", KBLU);
      } else {
        printf("%s-", KMAG);
        printf("%s", KNRM);
      }
    }
    printf("%s|", KNRM);
    printf("\n");
  }
}

void affichageTableauVictoire(int laVictoire,int joueurVainqueur) {
  l = 0;
  c = 0;
  if (laVictoire == 1) {
    if(joueurVainqueur==1){
      printf("%s  1 2 3 \n", KNRM);
      printf("%s1|%sX%s|%sX%s|%sX%s|\n", KNRM, KGRN, KNRM, KGRN,
      KNRM, KGRN, KNRM);
    }
    else{
      printf("%s  1 2 3 \n", KNRM);
      printf("%s1|%sO%s|%sO%s|%sO%s|\n", KNRM, KGRN, KNRM, KGRN,
      KNRM, KGRN, KNRM);
    }
    for (l = 1; l < 3; l++) {
      printf("%d", l + 1);
      for (c = 0; c < 3; c++) {
        printf("%s|", KNRM);
        if (tab[l][c] == 1) {
          printf("%sX", KRED);
        } else if (tab[l][c] == 2) {
          printf("%sO", KBLU);
        } else {
          printf("%s-", KMAG);
          printf("%s", KNRM);
        }
      }
      printf("%s|", KNRM);
      printf("\n");
    }
  }


  
}
void tourJ1() {
  printf("%sVous êtes le joueur X \n", KRED);
  int choixLigne;
  printf("Entrer ligne: ");
  scanf("%d", &choixLigne);
  if (choixLigne == 1 || choixLigne == 2 || choixLigne == 3) {
    int choixColonne;
    printf("Entrer colone:");
    scanf("%d", &choixColonne);
    if (choixColonne == 1 || choixColonne == 2 || choixColonne == 3) {
      if (tab[choixLigne - 1][choixColonne - 1] == 9) {
        tab[choixLigne - 1][choixColonne - 1] = 1;
      } else {
        printf("Case déjà prise \n");
        tourJ1();
      }
    } else {
      printf("Veuillez jouer des nombres logiques \n");
      tourJ1();
    }
  } else {
    printf("Veuillez jouer des nombres logiques \n");
    tourJ1();
  }
}

void tourJ2() {
  printf("%sVous êtes le joueur O \n", KBLU);
  int choixLigne;
  printf("Entrer ligne: ");
  scanf("%d", &choixLigne);
  if (choixLigne == 1 || choixLigne == 2 || choixLigne == 3) {
    int choixColonne;
    printf("Entrer colone:");
    scanf("%d", &choixColonne);
    if (choixColonne == 1 || choixColonne == 2 || choixColonne == 3) {
      if (tab[choixLigne - 1][choixColonne - 1] == 9) {
        tab[choixLigne - 1][choixColonne - 1] = 2;
      } else {
        printf("Case déjà prise \n");
        tourJ2();
      }
    } else {
      printf("Veuillez jouer des nombres logiques \n");
      tourJ2();
    }
  } else {
    printf("Veuillez jouer des nombres logiques \n");
    tourJ2();
  }
}

// CONDITIONS VICTOIRES
bool isMatchNul() {
  if (nbJeu == 9) {
    return true;
  } else {
    return false;
  }
}

bool victoireLigneJ1() {
  if ((tab[0][0] + tab[0][1] + tab[0][2]) == 3) {
    return true;
  } else if ((tab[0][0] + tab[0][1] + tab[0][2]) == 3) {
    return true;
  } else if ((tab[0][0] + tab[0][1] + tab[0][2]) == 3) {
    return true;
  }
  return false;
}

bool victoireColonneJ1() {
  if ((tab[0][0] + tab[1][0] + tab[2][0]) == 3) {
    return true;
  } else if ((tab[0][1] + tab[1][1] + tab[2][1]) == 3) {
    return true;
  } else if ((tab[0][2] + tab[1][2] + tab[2][2]) == 3) {
    return true;
  }

  return false;
}

int victoireDiagJ1() {
  if ((tab[0][0] + tab[1][1] + tab[2][2]) == 3) {
    return 1;
  } else if ((tab[2][0] + tab[1][1] + tab[0][2]) == 3) {
    return 2;
  }
  return 0;
}

bool victoireLigneJ2() {
  if ((tab[0][0] + tab[0][1] + tab[0][2]) == 6) {
    return true;
  } else if ((tab[0][0] + tab[0][1] + tab[0][2]) == 6) {
    return true;
  } else if ((tab[0][0] + tab[0][1] + tab[0][2]) == 6) {
    return true;
  }

  return false;
}

bool victoireColonneJ2() {
  if ((tab[0][0] + tab[1][0] + tab[2][0]) == 6) {
    return true;
  } else if ((tab[0][1] + tab[1][1] + tab[2][1]) == 6) {
    return true;
  } else if ((tab[0][2] + tab[1][2] + tab[2][2]) == 6) {
    return true;
  }

  return false;
}

bool victoireDiagJ2() {
  if ((tab[0][0] + tab[1][1] + tab[2][2]) == 6) {
    return true;
  } else if ((tab[2][0] + tab[1][1] + tab[0][2]) == 6) {
    return true;
  }
  return false;
}

joueur1(){
    while(1){
        read(sock,mess2,80);
        printf("%s",mess2);
        write(sock,tab,sizeof(tab));
        printf("Au tour du joueur 2\n");
    }
}

joueur2(){
    while(1){
        printf("Au tour du joueur 1\n");
        read(sock,mess2,80);
        printf("%s",mess2);
        write(sock,tab,sizeof(tab));
    }
}

int main(void) {
  creer_socket(); 
    // connexion au serveur 
    if(connect(sock, &serv_addr, sizeof(serv_addr)) < 0){
        perror("Connexion impossible");
        exit(1);
    }    // connexion à l'application du dessus 
    read(sock,mess2,80);
    printf("%s\n",mess2);
    read(sock,mess2,1);
    if(strncmp(mess2,"1",1) == 0){
        idClient = 1;
        printf("Vous etes le joueur 1\n");
    }
    else{
        idClient = 2;
        printf("Vous etes le joueur 2\n");
    }

  creationTableau();
  while (!isMatchNul()) {

    affichageTableau();
    printf("aaaaa\n");
    if(idClient == 1){
      tourJ1();
      write(sock,tab,sizeof(tab));
    }
    else{
      printf("Au tour du joueur 1\n");
        read(sock,mess2,80);
        tab = mess2;
    }

    nbJeu++;

    if (isMatchNul()) {
      printf("\n Match Nul \n");
      affichageTableau();
      break;
    } else if (victoireLigneJ1() || victoireColonneJ1() || victoireDiagJ1()) {
      printf("\n JOUEUR 1 à gagné \n");
      if (victoireLigneJ1() > 0) {
        if (victoireLigneJ1() == 1) {
          affichageTableauVictoire(1,1);
        }
      } else if (victoireColonneJ1()) {

      } else {
        if (victoireDiagJ1() == 1) {
          affichageTableauVictoire(7,1);
        } else {
          affichageTableauVictoire(8,1);
        }
      }
      break;
    }

    affichageTableau();
    if(idClient == 2){
      tourJ2();
      write(sock,tab,sizeof(tab));
    }
    else{
      printf("Au tour du joueur 2\n");
        read(sock,mess2,80);
        tab = mess2;
    }

    if (victoireLigneJ2() || victoireColonneJ2() || victoireDiagJ2()) {
      printf("\n JOUEUR 2 à gagné \n");
      if (victoireLigneJ2() > 0) {
        if (victoireLigneJ2() == 1) {
          affichageTableauVictoire(1,2);
        }
      } else if (victoireColonneJ2()) {

      } else {
        if (victoireDiagJ2() == 1) {
          affichageTableauVictoire(7,2);
        } else {
          affichageTableauVictoire(8,2);
        }
      }
      break;
    }

    nbJeu++;
  }

  return 0;
}
