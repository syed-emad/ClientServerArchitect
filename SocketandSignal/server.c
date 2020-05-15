 

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */


/*

filename server_ipaddress portno
argv[0] filename
argv[1] server_ipadress
argv[2] port no
#define _GNU_SOURCE
*/
#define _GNU_SOURCE
#include <stdio.h>
#include<wait.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include<signal.h>
const int size= 1000;
char delimiter[1]={' '};
char * token;
int n=0;
int number;
int sigpid=0;
void functions(int);

void Add();
void Sub();
void Mul();
void Div();
void Run();
void List();
void Kill();
void killall();
void Header();
void Print();

  


void error(const char *msg)
{
    perror(msg);
    exit(1);
}
typedef struct details
{
    int pid;
    int ppid;
    char name[1000];
    bool isActive;
    time_t startingtime;
    time_t endingtime;
}details;


details list[100];
int counter=0;

void handler(int sig)
{
char buff[size];
char bufferx[size];
int counterc=0;


   if (sig == SIGINT) {
    write(STDOUT_FILENO, "\nCTRL-C Entered\n", sizeof("\nCTRL-C Entered\n"));
   }
   
   else if (sig==SIGCHLD){
pid_t pid;
pid = wait(NULL);
//printf("Pid %d exit if (signo == SIGINT) {ed.\n", pid);
write(STDOUT_FILENO,"PID of Program: ",sizeof("PID of Program: "));
int x=sprintf(bufferx,"%d \n",pid);
write(STDOUT_FILENO,bufferx,x);
signal(SIGCHLD, handler);



       
    //signal(SIGTERM,handler); 
    for (int i = 0; i < counter; ++i)
    {   if (list[i].pid==pid )
        {    
            counterc++;
            
            if (list[i].isActive==true)
            {
                    //kill(list[i].pid, SIGKILL);
                    list[i].isActive=false;
                    list[i].endingtime=time(NULL);
                     
                    write(STDOUT_FILENO,"Program Closed Externally \n",sizeof("Program Closed Externally \n"));
              //int v=sprintf(buff,"Killed \n");
            //write(x,buff,v);
                    break;
             }
            //else{
             
            //write(STDOUT_FILENO,"Programm already Killed \n",sizeof("Programm already Killed \n"));
              //   }
        
        }


}
}
  

return;}
int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno,clilen,pid;
     //socklen_t clilen;
     //char buffer[255];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     
     //create Socket
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0){ 
        error("ERROR opening socket");
        exit(1);
    }

     //bzero((char *) &serv_addr, sizeof(serv_addr)); 
     portno = atoi(argv[1]);

     //populating structure
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     //Bind api
     int bindapi=bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) ;
     if (bindapi< 0) 
             {
            perror("Error at binding ");
            exit(1);
        }

       //listen to connections 
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     //signal(SIGTERM,sighandler);
    signal(SIGCHLD,handler); 
    signal(SIGINT,handler); 
    //signal(SIGALRM,handler);
   signal(SIGUSR1,handler);
   
     while(1)
     { newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
            pid = fork();
                if (pid<0){
                    perror("Error at Fork Api ");
                    exit(1);
                }

                if (pid == 0) {
                    //signal(SIGCHLD, handler);
                    close(sockfd);
                    // call main function
                     
                    functions(newsockfd);
                    
                }
                else {
                    close(newsockfd);
                } //waitpid(pid, NULL, 0); 

     }
      
     return 0; 
}



void functions(int x){
char buffer[size];
char displaybuffer[size];
  //signal(SIGTERM,sighandler);
	while(1){
//char buffer2[64]="PID     PPID    Name          Active     StartTime     EndTime\n";
           //bzero(buffer,256);
           n = read(x,buffer,255);
           if (n < 0) {error("ERROR reading from socket");exit(1);}
           write(STDOUT_FILENO,buffer,strlen(buffer)+1);
            char string[n - 1];
            sscanf(buffer, "%[^\n]", string);

            token = strtok(string, delimiter);
            
            if (strcmp(token, "add") == 0) {
               Add(x);}
            else if (strcmp(token, "sub") == 0) {
               Sub(x);}  	

	        else if (strcmp(token,"run")==0)
	        {
	            Run(x);
	        }
	        else if (strcmp(token,"list")==0)
	        {    //write(x,buffer2,64);
	            Header(x);
              List(x);
	        }
	        else if (strcmp(token,"kill")==0)
	        {
	            Kill(x);
	        }
	        else if (strcmp(token,"mul")==0)
	        {
	            Mul(x);
	        }
	        else if(strcmp(token,"div")==0){
	            Div(x);
	        }
          else if(strcmp(token,"print")==0){
            Print(x);
            //printf("aaa%s\n", buffer);
          }
	        else if(strcmp(token,"exit")==0){
	           killall(x);
	             
	        }
	        else if(strcmp(token,"killall")==0){
	            killall(x);
	        }
		      else {
                 n = sprintf(buffer, "Invalid Command \n");
                 write(x, buffer, n);
            } 
	}

	
}
void Print(int x){
char buffer[size];
char buffer2[9]={"sucess \n"};
  while((token=strtok(NULL,delimiter))!=NULL){
            printf("%s\n",token );
            write(STDOUT_FILENO,"Output From Print Command: ",sizeof("Output From Print Command: "));
            int a= sprintf(buffer,"%s\n",token);
            write(STDOUT_FILENO,buffer,a);
        }
  write(x,buffer2,9) ;
}
void Add(int x){
char buffer[size];
  int sum=0;
  
         while((token=strtok(NULL,delimiter))!=NULL){
            sum=sum+atoi(token);
        }
       
        int v=sprintf(buffer,"The answer is : %d \n",(int)sum);
        write(x,buffer,v);
          

    }
void Sub(int x){

  int sum=atoi(strtok(NULL,delimiter));
  char buffer[size];
   
         while((token=strtok(NULL,delimiter))!=NULL){
            sum=sum-atoi(token);
        }
      
        int v=sprintf(buffer,"The answer is : %d \n",(int)sum);
        write(x,buffer,v);
      
}
void Div(int x ){

  int sum=atoi(strtok(NULL,delimiter));
  char buff[100];
  char buffer[size];
 // int pipefd2[2];
  //pipe(pipefd2);  
  //int pid=fork();
  //if (pid==0)
    //  {     close(pipefd2[0]);
         while((token=strtok(NULL,delimiter))!=NULL){
            sum=sum/atoi(token);
        }

       // int v2=sprintf(buff,"%d",sum);
        //write(STDOUT_FILENO,buff,v2);
        int v=sprintf(buffer,"The answer is : %d \n",(int)sum);
        write(x,buffer,v);
    }
   // if (pid>0)
    //{
      //  int p=read(pipefd2[0],buff,100);
        //write(STDOUT_FILENO,"Answer: ",sizeof("Answer: "));
        //write(STDOUT_FILENO,buff,p);

        //int v=sprintf(buffer,"The answer is : %s \n",buff);
        //write(x,buffer,v);
        //write(STDOUT_FILENO,"\n",2);
       //     int *status;
       // int w = waitpid(pid, status, 0);   

    //}
//}
void Mul(int x){

  int sum=atoi(strtok(NULL,delimiter));
  char buff[100];
  char buffer[size];
//  int pipefd2[2];
 // pipe(pipefd2);  
 // int pid=fork();
 // if (pid==0)
   //   {     close(pipefd2[0]);
         while((token=strtok(NULL,delimiter))!=NULL){
            sum=sum*atoi(token);
        }
           int v=sprintf(buffer,"The answer is : %d \n",(int)sum);
        write(x,buffer,v); 
     //   int v=sprintf(buff,"%d",sum);
       // write(pipefd2[1],buff,v);
    //}
    //if (pid>0)
   // {
       // int p=read(pipefd2[0],buff,100);
     //   write(STDOUT_FILENO,"Answer: ",sizeof("Answer: "));
       // write(STDOUT_FILENO,buff,p);
       // write(STDOUT_FILENO,"\n",2);   
        //    int *status;
        //int w = waitpid(pid, status, 0);

    //}
}
 void Run(int x){
    int sum=0;
  char pID[100];
  char buff[100];
  int pipefd1[2];
  int idholder;
  int pipeN[2];
  char global[2]={'1'};
   
  pipe(pipefd1);
  pipe(pipeN);

  
  while ((token = strtok(NULL, delimiter)) != NULL) 
  { 
    int pipefdv[2];
    //pipe(pipefdv );
    pipe2(pipefdv, O_CLOEXEC );
    int pid=fork();
    
    if (pid==0)
        {   close(pipefd1[0]);
            close(pipefdv[0]);
            int c= sprintf(pID,"%d",getpid());
            write(pipefd1[1],pID,c);
             
            char *args[]={token,NULL};
         
            
            if (execvp(args[0],args)!=0)
            {  
              write(STDOUT_FILENO,"file not found in directoy",sizeof("file not found in directoy"));
               
              sprintf(global,"%d",3);
              write(pipefdv[1],global,3);
 
            }
        }

    
        if (pid>0)
            {  

                close(pipefd1[1]);
                close(pipefdv[1]);
                read(pipefdv[0],global,3);
                
            
                if (strcmp(global,"3")==0)
                {
                  printf("%s\n","Restart Program" );
                  exit(0);
                }
                else if (strcmp(global,"1")==0)
                {
                  int p=read(pipefd1[0],buff,14);
                
               
 					 
               list[counter].pid=atoi(buff);
               list[counter].ppid=getpid();
                
               strcpy( list[counter].name, token);
               list[counter].isActive=true;
               list[counter].startingtime=time(NULL); 
               counter++;
               
                  int v=sprintf(buff,"Opened \n");
        		write(x,buff,v);
                }
             





           }
       }

   }
void Header(int x){char buffer2[66]="PID     PPID    Name          Active     StartTime     EndTime\n\n";
    write(x,buffer2,64); 
}
void List(int x){
    char buffer[size];
    //write(x,buffer2,64);       

     //write(STDOUT_FILENO,"PID     PPID    Name          Active     StartTime     EndTime\n",64);
    //int number2=sprintf(buffer2,,"PID     PPID    Name          Active     StartTime     EndTime\n")

for (int i = 0; i <counter; ++i)
{  //write(x,buffer2,1000); 
   number= sprintf(buffer,"%d\t%d\t%s\t\t%d \t %ld \t %ld \n",list[i].pid,list[i].ppid,list[i].name,list[i].isActive,list[i].startingtime,list[i].endingtime);
   //write(STDOUT_FILENO,buffer,number);------UN COMMENT THIS TO SEE LIST CORRECTLY IN THE SERVER
   write(x,buffer,number);
}
 //int v=sprintf(buff,"Opened \n");
        		
}
void Kill(int x){  

    token=strtok(NULL,delimiter);
    char buff[size];

    int counterc=0;
  

	     
    //signal(SIGTERM,handler); 
    for (int i = 0; i < counter; ++i)
    {   if (list[i].pid==atoi(token) || strcmp(list[i].name,token)==0)
        {    
            counterc++;
            
            if (list[i].isActive==true)
            {
                    kill(list[i].pid, SIGKILL);
                    list[i].isActive=false;
                    list[i].endingtime=time(NULL);
                     
                    write(STDOUT_FILENO,"Successfully Exited \n",sizeof("Successfully Exited \n"));
                     int v=sprintf(buff,"Killed \n");
        		write(x,buff,v);
                    break;
             }
            else{
             
            write(STDOUT_FILENO,"Programm already Killed \n",sizeof("Programm already Killed \n"));
                 }
        
        }

    
    }        if(counterc==0){
         
        write(STDOUT_FILENO,"Incorrect PID entered \n",sizeof("Incorrect PID entered \n"));
    }

}

void killall(int x){
	char buff[size];
   int v=sprintf(buff,"Everything Killed ,Connection to server lost \n");
    write(x,buff,v);

     kill(0,SIGKILL);

  
}
 


 