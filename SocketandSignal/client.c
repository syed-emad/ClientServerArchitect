 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
const int size = 1000;
char delimiter[1] = {' '};
char * token;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[size];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0) {
            perror("Error at Socket Api ");
            exit(1);
        }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(1);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    
    serv_addr.sin_port = htons(portno);
    //connect
   int connection=connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)); 
    if (connection< 0){ 
        error("ERROR connecting API");
        exit(1);}

    n=0;
    while(1)
    {    

        bzero(buffer,256);
        //fgets(buffer,255,stdin);

        n=write(STDOUT_FILENO,"Enter Command: ",sizeof("Enter Command: "));
            if (n<0)
            {
                error("Error at write API");
                exit(1);
            }
             if (n<0)
            {
                error("Error at write API");
                exit(1);
            }
        n=read(STDIN_FILENO,buffer ,size);
        

           
        n = write(sockfd,buffer,strlen(buffer)+1);
        
        if (n < 0) {
             error("ERROR writing to socket");
            exit(1);
                }
        //bzero(buffer,256);
         
        n = read(sockfd,buffer,size);
        if (n < 0) {
             error("ERROR reading from socket/receiving data");
             exit(1);
         }

        //printf("Server asdsad: %s",buffer);
        //if(strcmp())
        write(STDOUT_FILENO,"Server: \n",sizeof("Server: "));
        write(STDOUT_FILENO,buffer,strlen(buffer) + 1);
        
    }


    
    return 0;
}
