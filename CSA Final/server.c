#define _GNU_SOURCE
#include <stdio.h>
#include <wait.h>
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
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
 #include <ctype.h>
//Global Variable
const int size = 1000;
char delimiter[1] = {' '};
char *token;
int n = 0;
int number;
int sigpid = 0;
int newsockfd = 0;
int portno = 0;
int counter = 0;
int counterserver = 1;
int pipefd1[2];
int pipefd2[2];
char buff[100];
char buf3[1024];
int pipefde[2];

//Functions
void functions(int);
void serverfunctions();
void Add();
void Sub();
void Mul();
void Div();
void Run();
void List();
void Kill();

void Exit();
void killall();
void Header();
void Print();
void ListAllClients();
void PrintOnClient();
void PrintOnAllClient();
void ListAllClientProcess();
void ListClientProcess();
void List2();
void ListIndClien();
void serverfunctions2(int pipefd);
void provide_communication(int serverWriteFd, int pipe_server, int pipewrite);
void serve_server2(int pipe_server2);

//Error Function
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

//List ClientSide
typedef struct details
{
    int pid;
    int ppid;
    char name[1000];
    bool isActive;
    time_t startingtime;
    time_t endingtime;
   time_t elapsed_time;
} details;
details list[100];

//List ServerSide
typedef struct details2
{
    int pid;
    int sockfd;
    char ip_address[100];
    int PipeFdWrite[2];
    int PipeFdRead[2];

} details2;
details2 list2[100];

struct args
{
    char *buff;
    int a;
};

//Signal Handler
void handler(int sig)
{
    char buff[size];
    char bufferx[size];
    int counterc = 0;

    if (sig == SIGINT)
    {
        write(STDOUT_FILENO, "\nCTRL-C Entered\n", sizeof("\nCTRL-C Entered\n"));
    }
    
    else if (sig == SIGCHLD)
    {
        pid_t pid;
        pid = wait(NULL);
        write(STDOUT_FILENO, "PID of Program Killed: ", sizeof("PID of Program Killed: "));
        int x = sprintf(bufferx, "%d \n", pid);
        write(STDOUT_FILENO, bufferx, x);
        signal(SIGCHLD, handler);

        //signal(SIGTERM,handler);
        for (int i = 0; i < counter; ++i)
        {
            if (list[i].pid == pid)
            {
                counterc++;

                if (list[i].isActive == true)
                {
                    
                    list[i].isActive = false;
                    list[i].endingtime = time(NULL);

                    write(STDOUT_FILENO, "Program Closed Externally \n", sizeof("Program Closed Externally \n"));
                    
                    break;
                }
                
            }
        }
    }

    return;
}


//*******THREADS*********//
void *communication_function(void *param)
{
    int *serve_array = (int *)param;
    provide_communication(serve_array[1], serve_array[0],serve_array[2]);
} 

void *serving(void *param)
{
     //int *serve_array = (int *)param;
     serverfunctions();
}

void *serving2(void *param)
{
  int *serve_array = (int *)param;
  serverfunctions2(serve_array[0]);
}
//Client Handler Thread
void *client_thread(void *parameter)
{ //thread

    functions(newsockfd);
}
//Server Function Thred
void *server_thread(void *param)
{    
  pthread_t newthread,newnewthread; 
  int thread_value = pthread_create(&newthread, NULL, serving, (void *)param);
  thread_value = pthread_create(&newnewthread, NULL, serving2, (void *)param); 
  pthread_join(newnewthread,NULL);
  pthread_join(newthread,NULL);    
}

//******THREAD FUNCTION CALLS********//
void provide_communication(int serverWriteFd, int pipe_server, int pipewrite)
{    char bufferMsg[size];
    char buffer2[size];
    char operation[10];
    char *token;
    char read_pipe[1000];
    char readbuff[100];
    int v;
    write(STDOUT_FILENO, "\n", 2);
    while (1)
    {
        int pipe_server_read = read(pipe_server, read_pipe, 1000);
        token = strtok(read_pipe, delimiter);
        if (strcmp(token, "listall") == 0)
        {
            if (counter==0)
            {
                char Message[]={"All Clients Havent Run Any Process.There List is Empty.\n"};
                write(STDOUT_FILENO, Message, sizeof(Message)-1);
            }
            else{
              for (int i = 0; i < counter; ++i)
                {
                  time_t st=list[i].startingtime;
                  struct tm *local = localtime(&st);
                  char starting_time[20];
                  sprintf(starting_time,"%02d:%02d:%02d",local->tm_hour,local->tm_min,local->tm_sec); 
                  time_t et=list[i].endingtime;
                  struct tm *et_local = localtime(&et);
                  char endingtime[20];
                  sprintf(endingtime,"%02d:%02d:%02d",et_local->tm_hour,et_local->tm_min,et_local->tm_sec);
                  time_t elapsed_t=list[i].elapsed_time;
                  struct tm *elapsed_local = localtime(&elapsed_t);
                  char elapsed_time[20];
                  sprintf(elapsed_time,"%02d:%02d:%02d",elapsed_local->tm_hour,elapsed_local->tm_min,elapsed_local->tm_sec);
                number = sprintf(buffer2, "%d\t%d\t%s\t\t%d \t %s \t %s \t  %s\n", list[i].pid, list[i].ppid, list[i].name, list[i].isActive, starting_time, endingtime,elapsed_time);
                int pipe = pipewrite;
                write(STDOUT_FILENO, "PID   X   PPID              Name               Active         StartTime         EndTime       ElapsedTime\n\n", sizeof("PID H     PPID              Name               Active         StartTime         EndTime       ElapsedTime\n\n"));

                write(pipe, buffer2, number);
                }
          }
        }
        else if (strcmp(token, "list") == 0)
        {
            token = strtok(NULL, delimiter);
            if (token==NULL)
            { //int pipe2 = pipewrite;
              write(STDOUT_FILENO,"PID MISSING\n",sizeof("PID MISSING\n"));
              //write(pipe2 ,"Missing\n",sizeof("Missing\n"));
              
            }
            else{

            if (counter==0)
            {
                char Message[]={"All Clients Havent Run Any Process.There List is Empty.\n"};
                write(STDOUT_FILENO, Message, sizeof(Message)-1);
            }
            for (int i = 0; i < counter; ++i)
            {
                if (list[i].ppid == atoi(token))
                  {time_t st=list[i].startingtime;
                    struct tm *local = localtime(&st);
                    char starting_time[20];
                    sprintf(starting_time,"%02d:%02d:%02d",local->tm_hour,local->tm_min,local->tm_sec); 
                    time_t et=list[i].endingtime;
                    struct tm *et_local = localtime(&et);
                    char endingtime[20];
                    sprintf(endingtime,"%02d:%02d:%02d",et_local->tm_hour,et_local->tm_min,et_local->tm_sec);
                   time_t elapsed_t=list[i].elapsed_time;
                  struct tm *elapsed_local = localtime(&elapsed_t);
                  char elapsed_time[20];
                  sprintf(elapsed_time,"%02d:%02d:%02d",elapsed_local->tm_hour,elapsed_local->tm_min,elapsed_local->tm_sec);
                number = sprintf(buffer2, "%d\t%d\t%s\t\t%d \t %s \t %s \t  %s\n", list[i].pid, list[i].ppid, list[i].name, list[i].isActive, starting_time, endingtime,elapsed_time);
                                    int pipe = pipewrite;
                    write(STDOUT_FILENO, "PID H     PPID              Name               Active         StartTime         EndTime       ElapsedTime\n\n", sizeof("PID H     PPID              Name               Active         StartTime         EndTime       ElapsedTime\n\n"));
                    write(pipe ,buffer2, number);
                }
            }}
        }
       
        else if (strcmp(token, "printall") == 0)
        {
            char *c = strtok(NULL, "");  

            
            int sc = serverWriteFd;
            
                 v = sprintf(bufferMsg, "Message From Server : %s \n", c);
                write(sc, bufferMsg, strlen(bufferMsg));
                //memset(bufferMsg);
                       }
        else if (strcmp(token, "print") == 0)
        {
            token = strtok(NULL, delimiter);
            if (serverWriteFd == atoi(token))
            { 
                char *c = strtok(NULL, "");
                int sc = serverWriteFd;
                v = sprintf(bufferMsg, "Message From Server : %s \n", c);
                write(sc, bufferMsg, strlen(bufferMsg));
                //memset(bufferMsg);
            }
              else{
                
              write(STDOUT_FILENO,"INVALID FD\n",sizeof("INVALID FD\n"));
               
              }
        }
        else if (strcmp(token, "exit") == 0)
        {
            int sc = serverWriteFd;
            write(sc, "exit", 4);  
        }
        else{
                write(STDOUT_FILENO,"INVALID INPUT\n",sizeof("INVALID INPUT\n"));
        }
    }
}
void serverfunctions2(int pipefd){ 
  char read_pipe[1000];
  while(1){
         int pipe_server_read = read(pipefd, read_pipe, size);
         write(STDOUT_FILENO,read_pipe,pipe_server_read);
  }
}

void ListIndClien()
{
    char buffersock2[size];
    token = strtok(NULL, delimiter);
    //printf("%s\n", token);
    
    write(STDOUT_FILENO, "PID            SockFD           IP Address        PipeFdWrite[0]         PipeFdWrite[1]          PipeFdRead[0]        PipeFdRead[1]  \n", sizeof("PID            SockFD           IP Address        PipeFdWrite[0]         PipeFdWrite[1]          PipeFdRead[0]        PipeFdRead[1]  \n"));

    for (int i = 1; i < counterserver; ++i)
    { //write(x,buffer2,1000);
        if (list2[i].pid == atoi(token))
        {
           int number = sprintf(buffersock2, "%d\t\t %d\t\t %s\t\t  %d\t\t\t  %d\t\t\t  %d\t\t %d\t\t", list2[i].pid, list2[i].sockfd,list2[i].ip_address, list2[i].PipeFdWrite[0], list2[i].PipeFdWrite[1], list2[i].PipeFdRead[0], list2[i].PipeFdRead[1]);

            write(STDOUT_FILENO, buffersock2, number);
            write(STDOUT_FILENO, "\n", 2);
        }
    }   
}

void ListAllClients()
{  
    char buffersock2[size];
    write(STDOUT_FILENO, "PID            SockFD           IP Address        PipeFdWrite[0]         PipeFdWrite[1]          PipeFdRead[0]        PipeFdRead[1]  \n", sizeof("PID            SockFD           IP Address        PipeFdWrite[0]         PipeFdWrite[1]          PipeFdRead[0]        PipeFdRead[1]  \n"));
    for (int i = 1; i < counterserver; ++i)
    { //write(x,buffer2,1000);
        int number = sprintf(buffersock2, "%d\t\t %d\t\t %s\t\t  %d\t\t\t  %d\t\t\t  %d\t\t %d\t\t", list2[i].pid, list2[i].sockfd,list2[i].ip_address, list2[i].PipeFdWrite[0], list2[i].PipeFdWrite[1], list2[i].PipeFdRead[0], list2[i].PipeFdRead[1]);

        write(STDOUT_FILENO, buffersock2, number);
        write(STDOUT_FILENO, "\n", 2);
    } 
}
 

void serverfunctions()
{
    char read_pipe[1000];
    char buffer[size];
    while(1){
    int a = read(STDIN_FILENO, buffer, size);
    buffer[a - 1] = '\0';        
    char holder[a];
    strcpy(holder, buffer);
    token = strtok(buffer, delimiter);
   if (a== -1)
        {
            error("Read failed");
        }

    if (a==1)
    {
      write(STDOUT_FILENO,"You Pressed Enter Without Any Input\n",sizeof("You Pressed Enter Without Any Input\n"));
          write(STDOUT_FILENO,"Enter Command(Server)\n",sizeof("Enter Command(Server)\n"));
    }
    else if   (strcasecmp(token, "allclientlist") == 0)
    {
        ListAllClients();
    }
   else if   (strcasecmp(token, "clientlist") == 0)
    {
        ListIndClien();
    }
    else if   (strcasecmp(token, "listall") == 0||strcasecmp(token, "list") == 0||strcasecmp(token, "print") == 0||strcasecmp(token, "printall") == 0|| strcasecmp(token, "exit") == 0)
    {
        for (int i = 1; i < counterserver; i++)
        {

            int pipe = list2[i].PipeFdWrite[1];
            
            write(pipe, holder, a);
        }
        
    }
    else{write(STDOUT_FILENO,"Invalid Command Entered..\n",sizeof("Invalid Command Entered..\n"));
          write(STDOUT_FILENO,"Enter Command(Server)\n",sizeof("Enter Command(Server)\n"));
        }

        }   
}

//Main Code
int main(int argc, char *argv[])
{   
    int sockfd, clilen, pid;
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    //create Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //if not successfuly created Give Error
    if (sockfd < 0)
    {
        error("ERROR opening socket");
        exit(1);
    }

    portno = atoi(argv[1]);

    //populating structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
     
    int bindapi = bind(sockfd, (struct sockaddr *)&serv_addr,
                       sizeof(serv_addr));

    //Error Msg is conection error
    if (bindapi < 0)
    {
        perror("Error at binding ");
        exit(1);
    }

    //listen to connections, 5 conections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
     

    //signal(SIGTERM,sighandler);
    signal(SIGCHLD, handler);
    signal(SIGINT, handler);
    signal(SIGKILL,handler);
    signal(SIGUSR1, handler);

    //Thread Connection
    pthread_t ServerFunc, ClientInputs, server_handle;
        write(STDOUT_FILENO,"Commands: 1.allclientlist 2.clientlist 3.printall 4.print 5.list 6.listall  \nSamples:\n1. allclientlist    //to see list of all clients connected \n2.clientlist 123(PID)   //to display specific client list \n3.printall msg   //to send msg to all clients \n4.print pid //to send msg to specifiic client \n5.list pid to print specific list \n6.listall //to print all list  \n",sizeof("Commands: 1.allclientlist 2.clientlist 3.printall 4.print 5.list 6.listall  \nSamples:\n1. allclientlist    //to see list of all clients connected \n2.clientlist 123(PID)   //to display specific client list \n3.printall msg   //to send msg to all clients \n4.print pid //to send msg to specifiic client \n5.list pid to print specific list \n6.listall //to print all list  \n"));

    while (1)
    {
        newsockfd = accept(sockfd,
                           (struct sockaddr *)&cli_addr,
                           &clilen);

        //if Accepting creates error print error
        if (newsockfd < 0)
            error("ERROR on accept");

        int pipe_server[2];
        int pipe_server2[2];
        pipe(pipe_server);
        pipe(pipe_server2);
        int thread_value;
        int array_pass1[1];
        array_pass1[0]=pipe_server2[0];
        thread_value = pthread_create(&ServerFunc, NULL, server_thread, (void *)array_pass1);
        pid = fork();
        //if error in fork
        if (pid < 0)

        {
            error("Error at Fork Api ");
            exit(1);
        }

        if (pid == 0)
        {
            close(sockfd);
            thread_value = pthread_create(&ClientInputs, NULL, client_thread, (void *)NULL);
             if(thread_value!=0)
                    {
                        perror("Thread Failed");
                    }
            int array_pass[4];
            array_pass[1] = newsockfd;
            array_pass[0] = pipe_server[0];
            array_pass[2]=pipe_server2[1];
            array_pass[3]=pipe_server2[0];

            thread_value = pthread_create(&server_handle, NULL, communication_function, (void *)array_pass);
              if(thread_value!=0)
                    {
                        error("Thread Failed");
                    }

            pthread_join(ServerFunc, NULL);
            pthread_join(server_handle, NULL);
            pthread_join(ClientInputs, NULL);
        }
        else
        {
            char buffersock[size];
            list2[counterserver].pid = pid;
            list2[counterserver].sockfd = newsockfd;
            list2[counterserver].PipeFdWrite[1] = pipe_server[1];
            list2[counterserver].PipeFdWrite[0] = pipe_server[0];
            list2[counterserver].PipeFdRead[0] = pipe_server2[0];
            list2[counterserver].PipeFdRead[1] = pipe_server2[1];
            char address_string[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET,&(cli_addr.sin_addr),address_string,INET6_ADDRSTRLEN);
            int port = (int) ntohs(cli_addr.sin_port);
            char ip_add[strlen(address_string)];
            strcpy(ip_add,address_string);
            strcpy(list2[counterserver].ip_address,address_string);
            counterserver++;
            write(STDOUT_FILENO,"Client Connected...\n",sizeof("Client Connected...\n"));
            write(STDOUT_FILENO, "Enter Command:", sizeof("Enter Command:"));  
        } 
    }
    return 0;
}
 
void functions(int x)
{
    char buffer[size];
    char displaybuffer[size];
    while (1)
    {
        n = read(x, buffer, 255);
        if (n < 0)
        {
            error("ERROR reading from socket");
            exit(1);
        }
        write(STDOUT_FILENO,"Client asked to:",sizeof("Client asked to:"));
        write(STDOUT_FILENO, buffer, strlen(buffer) + 1);
        write(STDOUT_FILENO,"\n",2);
        char string[n - 1];
        sscanf(buffer, "%[^\n]", string);

        token = strtok(string, delimiter);

        if (strcasecmp(token, "add") == 0)
        {
            Add(x);
        }
        else if (strcasecmp(token, "sub") == 0)
        {
            Sub(x);
        }

        else if (strcasecmp(token, "run") == 0)
        {
            Run(x);
        }
        else if (strcasecmp(token, "list") == 0)
        { //write(x,buffer2,64);
            Header(x);
            List(x);
        }
        else if (strcasecmp(token, "kill") == 0)
        {
            Kill(x);
        }
        else if (strcasecmp(token, "mul") == 0)
        {
            Mul(x);
        }
        else if (strcasecmp(token, "div") == 0)
        {
            Div(x);
        }
        else if (strcasecmp(token, "print") == 0)
        {
            Print(x);
            //printf("aaa%s\n", buffer);
        }
        else if (strcasecmp(token, "exit") == 0)
        {
            Exit(x);  
        }
        else if (strcasecmp(token, "killall") == 0)
        {
            killall(x);
        }
        else
        {
            n = sprintf(buffer, "Invalid Command \n");
            write(x, buffer, n);
        }
        write(STDOUT_FILENO,"Enter Command(In Server)\n",sizeof("Enter Command(In Server)\n"));  }
}
void Print(int x)
{
    char buffer[size];
    char buffer2[9] = {"sucess \n"};
    while ((token = strtok(NULL, delimiter)) != NULL)
    {
        printf("%s\n", token);
        write(STDOUT_FILENO, "Output From Print Command: ", sizeof("Output From Print Command: "));
        int a = sprintf(buffer, "%s\n", token);
        write(STDOUT_FILENO, buffer, a);
    }
    write(x, buffer2, 9);
}

void Add(int x)
{  
    char buffer[size];
    char buffer2[size];
    int sum = 0;
    while ((token = strtok(NULL, delimiter)) != NULL)
    {  
        bool flag=false;
       for (int i = 0; i < strlen(token); ++i)
    {
          if (isdigit(token[i])==0)
          {
             int v = sprintf(buff, "Non Numeric Character Detected\n" );
                write(x, buff, v);
            flag=true;
            
          }

    }
                if (flag==false)
          {
             sum=sum+atoi(token);
          }else{break;}
   
    }
    int v = sprintf(buffer, "The answer is(From Server) : %d \n", (int)sum);
    write(x, buffer, v);
}

void Sub(int x)
{

  char buffer[size];
    char buffer2[size];
    //int sum = 0;
     int sum = atoi(strtok(NULL, delimiter));
    while ((token = strtok(NULL, delimiter)) != NULL)
    {  
        bool flag=false;
       for (int i = 0; i < strlen(token); ++i)
    {
          if (isdigit(token[i])==0)
            {int v = sprintf(buff, "Non Numeric Character Detected\n" );
             write(x, buff, v);
            flag=true;
            
          }

    }
                if (flag==false)
          {
             sum=sum-atoi(token);
          }else{break;}
   
    }
    int v = sprintf(buffer, "The answer is(From Server) : %d \n", (int)sum);
    write(x, buffer, v);
}

void Div(int x)
{char buffer[size];
    char buffer2[size];
        int sum = atoi(strtok(NULL, delimiter));
    while ((token = strtok(NULL, delimiter)) != NULL)
    {  
        bool flag=false;
       for (int i = 0; i < strlen(token); ++i)
    {
          if (isdigit(token[i])==0)
          {
           int v = sprintf(buff, "Non Numeric Character Detected\n" );
           write(x, buff, v);
            flag=true;
            
          }

    }
                if (flag==false)
          {
             sum=sum/atoi(token);
          }else{break;}
   
    }
    int v = sprintf(buffer, "The answer is(From Server) : %d \n", (int)sum);
    write(x, buffer, v);
}

void Mul(int x)
{char buffer[size];
    char buffer2[size];
        int sum = atoi(strtok(NULL, delimiter));
    while ((token = strtok(NULL, delimiter)) != NULL)
    {  
        bool flag=false;
       for (int i = 0; i < strlen(token); ++i)
    {
          if (isdigit(token[i])==0)
          {
            int v = sprintf(buff, "Non Numeric Character Detected\n" );
            write(x, buff, v);
            flag=true;
            
          }

    }
                if (flag==false)
          {
             sum=sum*atoi(token);
          }else{break;}
   
    }
    int v = sprintf(buffer, "The answer is(From Server) : %d \n", (int)sum);
    write(x, buffer, v);
}

void Run(int x)
{
    int sum = 0;
    char pID[100];
    char buff[100];
    int pipefd1[2];
    int idholder;
    int pipeN[2];
    char global[2] = {'1'};

    pipe(pipefd1);
    pipe(pipeN);

    while ((token = strtok(NULL, delimiter)) != NULL)
    {
        int pipefdv[2];
        //pipe(pipefdv );
        pipe2(pipefdv, O_CLOEXEC);
        int pid = fork();

        if (pid == 0)
        {
            close(pipefd1[0]);
            close(pipefdv[0]);
            int c = sprintf(pID, "%d", getpid());
            write(pipefd1[1], pID, c);

            char *args[] = {token, NULL};

            if (execvp(args[0], args) != 0)
            {
                //write(STDOUT_FILENO, "file not found in directoy", sizeof("file not found in directoy"));
                int v = sprintf(buff, "File Not Found In Directoy" );
                write(x, buff, v); 
                sprintf(global, "%d", 3);
                write(pipefdv[1], global, 3);
            }
        }

        if (pid > 0)
        {

            close(pipefd1[1]);
            close(pipefdv[1]);
            read(pipefdv[0], global, 3);

            if (strcmp(global, "3") == 0)
            {
                int v = sprintf(buff, "Open Program Again" );
                write(x, buff, v); 
                //exit(0);
            }
            else if (strcmp(global, "1") == 0)
            {     time_t now;
                    time(&now);
                int p = read(pipefd1[0], buff, 14);

                list[counter].pid = atoi(buff);
                list[counter].ppid = getpid();

                strcpy(list[counter].name, token);
                list[counter].isActive = true;
                list[counter].startingtime = time(&now);
                counter++;

                int v = sprintf(buff, "Program Opened \n");
                write(x, buff, v);
            }
        }
    }
}

void Header(int x)
{
    //char buffer2[66] = ;
    write(x, "PID H     PPID              Name               Active         StartTime         EndTime       ElapsedTime\n\n", sizeof("PID H     PPID              Name               Active         StartTime         EndTime       ElapsedTime\n\n"));
}

void List(int x)
{  
    char buffer[size];
    for (int i = 0; i < counter; ++i)
      { time_t st=list[i].startingtime;
        struct tm *local = localtime(&st);
        char starting_time[20];
        sprintf(starting_time,"%02d:%02d:%02d",local->tm_hour,local->tm_min,local->tm_sec); 
        time_t et=list[i].endingtime;
        struct tm *et_local = localtime(&et);
        char endingtime[20];
        sprintf(endingtime,"%02d:%02d:%02d",et_local->tm_hour,et_local->tm_min,et_local->tm_sec);
          time_t elapsed_t=list[i].elapsed_time;
            struct tm *elapsed_local = localtime(&elapsed_t);
            char elapsed_time[20];
            sprintf(elapsed_time,"%02d:%02d:%02d",elapsed_local->tm_hour,elapsed_local->tm_min,elapsed_local->tm_sec);

        number = sprintf(buffer, "%d\t%d\t%s\t\t%d \t %s \t %s \t  %s\n", list[i].pid, list[i].ppid, list[i].name, list[i].isActive, starting_time, endingtime,elapsed_time);
        write(x, buffer, number);
    }   
}

void Kill(int x)
{

    token = strtok(NULL, delimiter);
    char buff[size];

    int counterc = 0;

    //signal(SIGTERM,handler);
    for (int i = 0; i < counter; ++i)
    { 
        if (list[i].pid == atoi(token) || strcmp(list[i].name, token) == 0)
        {

            if (list[i].isActive == true)
            {
                kill(list[i].pid, SIGKILL);
                list[i].isActive = false;
                //list[i].endingtime = time(NULL);
                time_t now;
                        list[i].endingtime=time(&now);
                        time_t st_time=list[i].startingtime;
                        time_t end_time=list[i].endingtime;
                        time_t elapsed_time=difftime(st_time,end_time);
                        list[i].elapsed_time=elapsed_time;
                write(STDOUT_FILENO, "Successfully Exited \n", sizeof("Successfully Exited \n"));
                int v = sprintf(buff, "Process Killed \n");
                write(x, buff, v);
                break;
            }
            else
            {

                write(STDOUT_FILENO, "Programm already Killed \n", sizeof("Programm already Killed \n"));
            }
        }
    }
    if (counterc == 0)
    {

        write(STDOUT_FILENO, "Incorrect PID entered \n", sizeof("Incorrect PID entered \n"));
    }
}
void Exit (int x)
{
token = strtok(NULL, delimiter);
    char buff[size];

    int counterc = 0;

    //signal(SIGTERM,handler);
    for (int i = 0; i < counter; ++i)
    {
       
           

            if (list[i].isActive == true)
            {
                kill(list[i].pid, SIGKILL);
                list[i].isActive = false;
                //list[i].endingtime = time(NULL);
                time_t now;
                        list[i].endingtime=time(&now);
                        time_t st_time=list[i].startingtime;
                        time_t end_time=list[i].endingtime;
                        time_t elapsed_time=difftime(st_time,end_time);
                        list[i].elapsed_time=elapsed_time;
                write(STDOUT_FILENO, "Successfully Exited \n", sizeof("Successfully Exited \n"));
               
                break;
            }
            else
            {

                write(STDOUT_FILENO, "Programm already Killed \n", sizeof("Programm already Killed \n"));
            }
      
    }
     int v = sprintf(buff, "Killed \n");
                write(x, buff, v);
    
}
void killall(int x)
{
    char buff[size];
    int v = sprintf(buff, "Everything Killed ,Connection to server lost \n");
    write(x, buff, v);
    kill(0, SIGKILL);
}