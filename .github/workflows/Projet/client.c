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


joueur1(){
    while(1){
        read(sock,mess2,80);
        printf("%s",mess2);
        gets(mess);
        write(sock,mess,80);
        printf("Au tour du joueur 2\n");
    }
}

joueur2(){
    while(1){
        printf("Au tour du joueur 1\n");
        read(sock,mess2,80);
        printf("%s",mess2);

        gets(mess);
        write(sock,mess,80);
    }
}
 
main() 
{ // creation socket 
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
        joueur1();
    }
    else{
        idClient = 2;
        printf("Vous etes le joueur 2\n");
        joueur2();
    }
    /*

    if(idClient == 1){
        while(1){
            read(sock,mess2,80);
            printf("%s\n",mess2);
            write(sock,"coucou",80);
            //write(sock,"loulou",80);
            //read(sock,mess2,80);
            break;
        }
    }
    else{
        while(1){
            int tab[3] = {4,6,3};
            int count = 3;
            read(sock,mess2,80);
            write(sock,"aaaa",80);
            write(sock,tab,sizeof(tab));
            printf("%s\n",mess2);
            printf("%d",tab[2]);
            
            break;
        }
    }
    */
    
}