 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>
const int size = 1000;
int sockfd;
char buffer[1000];
char delimiter[1] = {' '};
char * token;
struct arg_struct {
    int arg1;
    char *arg2;
};

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
 
void * write_thread(void *parameter) {


    while(true){

write(STDOUT_FILENO,"Enter Command: ",sizeof("Enter Command: "));
      
            int a =read(STDIN_FILENO,buffer,size);
             
            int n = write(sockfd,buffer,strlen(buffer)+1);

            if (n < 0) {
           error("ERROR writing to socket");
            

       }
        

       
 
         
     
      }
}
void * read_thread(void *parameter) {
      //struct arg_struct *args = arguments;
    
      while(true){
        int n = read(sockfd,buffer,size);
        write(STDOUT_FILENO,"\n",2);
        if (strcmp(buffer,"Killed \n")==0)
        {
            write(STDOUT_FILENO,"Client ",sizeof("Client "));
           kill(0, SIGKILL);

        }
         if (strcmp(buffer,"exit")==0)
        {
            write(STDOUT_FILENO,"Client Killed By Server",sizeof("Client Killed By Server"));
           kill(0, SIGKILL);

        }
        write(STDOUT_FILENO,buffer, n);//printf("%s\n","asd" );
        memset(buffer,'\0',1000);
    }
        
      //printf("%d\n", args -> arg1);
      //n = read(sockfd,buffer,size);
                  
    
}

int main(int argc, char *argv[])
{
    int  portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    

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
    
    write(STDOUT_FILENO,"Commands: 1.run 2.list 3.kill 4.add 5.sub 6.mul 7.div \nSamples:\n1. run gedit    //to run app \n2.list   //to display list \n3.kill 123   //to kill process by using its pid 123 here is pid \n  kill gedit //to kill process by using its name \n4.add 2 2//for addition, follow same method for sub, mul div \n",sizeof("Commands: 1.run 2.list 3.kill 4.add 5.sub 6.mul 7.div \nSamples:\n1. run gedit    //to run app \n2.list   //to display list \n3.kill 123   //to kill process by using its pid 123 here is pid \n  kill gedit //to kill process by using its name \n4.add 2 2//for addition, follow same method for sub, mul div \n"));


       

        pthread_t read, write;
       
        //struct arg_struct args;
         
         
        
        int b=pthread_create(&write,NULL,write_thread,(void *)NULL);
        if(b!=0){printf("%s\n","error" );}
        int c= pthread_create(&read,NULL,read_thread,(void *)NULL);
    
        pthread_join(read,NULL);
        pthread_join(write,NULL);
        
    



   return 0;
}
