#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#define PORT 1234
int sock, socketClient1, socketClient2, lg;
char mess1[80];
char mess2[80];

static  sem_t  Sem1;
static  sem_t  Sem2;

int nbJeu = 0;
int tab[3][3];

int isJoueurDeuxConnected = 0;

struct sockaddr_in local; // ???
struct sockaddr_in distant1; // ???
struct sockaddr_in distant2; // ???
creer_socket()
{
// preparation des champs d’entete
bzero(&local, sizeof(local)); // ???
local.sin_family = AF_INET; // Définition de la famille d'adresses
local.sin_port = htons(PORT); // ???
local.sin_addr.s_addr = INADDR_ANY; // ???
bzero(&(local.sin_zero),8); // ???
lg = sizeof(struct sockaddr_in);
// création d'une socket
if((sock=socket(AF_INET, SOCK_STREAM,0)) == -1){perror("socket"); exit(1);}
}

void joueur1(){
    

    if(listen(sock, 5) == -1){
        perror("listen");exit(1);
    }

    if((socketClient1=accept(sock, (struct sockaddr *)&distant1, &lg)) == -1)
    {perror("accept");exit(1);}

    printf ("Le premier joueur est connecte\n");
    write(socketClient1,"En attente d'un adversaire\n",80);
    write(socketClient1,"1",1);

    while(isJoueurDeuxConnected == 0){ }
    printf("Le deuxieme joueur est connecte\n");
    while(1){
        sem_wait(&Sem1);
        write(socketClient1,"A toi de jouer\n",80);
        read(socketClient1,mess1,80);
        printf("%s \n",mess1); //tableau du joueur 1
        printf("Le joueur 1 a joue\n");
        sem_post(&Sem2);
    
    }
}

void joueur2(){


    if(listen(sock, 5) == -1){
        perror("listen");exit(1);
    }
    if((socketClient2=accept(sock, (struct sockaddr *)&distant1, &lg)) == -1)
        {perror("accept");exit(1);}
        //printf ("Le deuxieme joueur est connecte\n");
        isJoueurDeuxConnected = 1;
        write(socketClient2,"Adversaire trouve !",80);
        write(socketClient2,"2",1);

        while(1){
            sem_wait(&Sem2);
            write(socketClient2,"A toi de jouer\n",80);
            read(socketClient2,mess2,80);
            printf("%s \n",mess2);
            printf("Le joueur 2 a joue\n");
            sem_post(&Sem1);  
        }
}
 

    

main(){
    sem_init(&Sem1, 0, 1);
    sem_init(&Sem2, 0, 0);
    creer_socket();
    // attribution d'un numero de port spécifique a une socket
    if(bind(sock, (struct sockaddr *)&local, sizeof(struct sockaddr)) == -1){
        perror("bind");exit(1);
    }

    pthread_t num_thread[2];
    if (pthread_create(&num_thread[0], NULL, (void *(*)())joueur1, NULL) == -1) 
        perror ("pb fork\n");
    if (pthread_create(&num_thread[1], NULL, (void *(*)())joueur2, NULL) == -1) 
        perror ("pb fork\n");

    for (int i=0; i<2;i++)  pthread_join(num_thread[i], NULL); 

    /*
    creer_socket();
    // attribution d'un numero de port spécifique a une socket
    if(bind(sock, (struct sockaddr *)&local, sizeof(struct sockaddr)) == -1){
        perror("bind");exit(1);
    }

    if(listen(sock, 5) == -1){
        perror("listen");exit(1);
    }

    while(1)
    { 
        if((socketClient1=accept(sock, (struct sockaddr *)&distant1, &lg)) == -1)
        {perror("accept");exit(1);}
            printf ("Le premier joueur est connecte\n");
            write(socketClient1,"En attente d'un adversaire\n",80); //1 write

                if(listen(sock, 5) == -1){
                    perror("listen");exit(1);
                }

                    if((socketClient2=accept(sock, (struct sockaddr *)&distant2, &lg)) == -1)
                    {perror("accept");exit(1);}
                    printf("Le deuxieme joueur est connecte\n");

                    while(1) {
                        write(socketClient2,"C'est au tour de ton adversaire\n",80);   //2 write
                        write(socketClient1,"A ton tour de jouer\n",80);  //2 write
                        read(socketClient1,mess1,80);
                        //write(socketClient1,"Bien recu\n",80);
                        read(socketClient1,mess2,80);
                        //write(socketClient1,"Bien recu\n",80);
                        printf("Coucous %s %s",mess1, mess2);
                        //write(socketClient2,c,80);
                        //write(socketClient2,l,80);
                        
                        break;
                    } 
                    printf("Fin");
                    break; 
                }   
                */
} 