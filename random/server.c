#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#define BUFFER_SIZE 1024
#define MAXCLIENTS 20

void* client_thread(void * arg);
void sendmessage(char * message, int sock);
void interpret_buffer(char * buf, int sock, int n);

int main(int argc, char ** argv){
  int port,sock,currentconnections = 0,sockdescriptors, * temp, rc;
  struct sockaddr_in server,client;
  socklen_t serverlen,clientlen;
  pthread_t threads[MAXCLIENTS];

  mkdir("./.storage",0777); //will attempt to make it.. Either way the dir will be made and not overwritten
  chdir("./.storage"); //all files will be here

  if(argc == 1){
    perror("error:port not specified");
    return EXIT_FAILURE;
  }
  port = atoi(argv[1]);
  if(port < 8000 || port > 9000){
    perror("error:port out of 8000-9000 range");
    return EXIT_FAILURE;
  }
  printf("Started file-server\nListening on port %d\n",port);
  
  //try to create a TCP socket
  sock = socket( PF_INET, SOCK_STREAM, 0 );
  if(sock < 0 ){
    perror( "socket() failed" );
    exit( EXIT_FAILURE );
  }

  bzero(&server, sizeof(server));
  //set the socket structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  //server.sin_addr.s_addr = htonl((((((127 << 8) | 0) << 8) | 0) << 8) | 1);
  server.sin_port = htons(port);
 
  //bind socket
  serverlen = sizeof(server);
  if(bind(sock,(struct sockaddr *)&server,serverlen) < 0){
    perror( "bind() failed" );
    exit( EXIT_FAILURE );
  }
  
  //make a listener
  listen(sock,MAXCLIENTS);
  printf( "Listener bound to port %d on addr %s\n", port , inet_ntoa( (struct in_addr)server.sin_addr ));

  //server loop to run forever
  while(true){
    printf( "PARENT: Blocked on accept()\n" );
    clientlen = sizeof(client);
    sockdescriptors = accept(sock,(struct sockaddr *)&client,
			     (socklen_t*)&clientlen);
    printf( "PARENT: Accepted client connection\n" );
    if(sockdescriptors  < 0){
      continue;
    }
    temp = (int *) malloc(sizeof(int));
    *temp = sockdescriptors;
    rc = pthread_create(&(threads[currentconnections]), NULL, client_thread,(void *) temp);
    if ( rc != 0 ){
      printf( "MAIN: Could not create child thread" );
      continue;
    }
    ++currentconnections;
  }
  close(sock);
  return EXIT_SUCCESS;
}

void* client_thread(void * arg){
  int csock = *(int*)arg,n;
  char buffer[BUFFER_SIZE];

  do{
    n = recv( csock, buffer, BUFFER_SIZE, 0 );
    if ( n < 0 ) {
      perror( "recv() failed" );
    } else if ( n == 0 ) {
      printf( "CHILD %d: Rcvd 0 from recv(); closing socket\n",
	      getpid() );
    } else {
      buffer[n] = '\0';  /* terminate buffer */
      printf( "Rcvd message:%s\n",
	      //inet_ntoa( (struct in_addr)client.sin_addr ),
	      buffer );
      interpret_buffer(buffer,csock,n);      
    }
  }while(n > 0);
  close(csock);
  free(arg);
  return NULL;
}

void interpret_buffer(char * buf, int sock, int n){
  char copy[BUFFER_SIZE], *ptr, *prev, readbuf[BUFFER_SIZE], temp[BUFFER_SIZE];
  FILE * file;
  int num;
  DIR * dir;
  struct dirent * dirfile;

  memcopy(copy,buf,n+1);
  ptr = strpbrk(copy," \\");
  if(!ptr){
    sendmessage("ERROR: <no such command>\n",sock);
    return;
  }

  //found what we think is a command...
  if(*ptr == '\\' && *(ptr+1) == 'n'){
    *ptr = '\0'; ptr+=2; 
  }else{
    *ptr = '\0'; ++ptr; 
  }
  prev = ptr;
  if(!strcmp(copy,"ADD")){
    //add command
    ptr = strpbrk(ptr," "); 
    *ptr = '\0'; ++ptr;
    file = fopen(prev,"rb");
    if(file){
      sendmessage("ERROR: file exist\n",sock);
      return;
    }
    file = fopen(prev,"wb");
    printf("%s\n",prev);
    
    prev = ptr; ptr = strpbrk(ptr,"\\"); 
    *ptr = '\0'; ptr+=2;
    num = atoi(prev);//lock file here
    fwrite(ptr,sizeof(char),num,file);
    sendmessage("ACK",sock);//release lock here
    fclose(file);
  }else if(!strcmp(copy,"APPEND")){
    //append command
    ptr = strpbrk(ptr," ");
    *ptr = '\0'; ++ptr;
    file = fopen(prev,"rb");
    if(!file){
      sendmessage("ERROR: file does not exist\n",sock);
      return;
    }
    fclose(file);
    file = fopen(prev,"ab");
    prev = ptr; ptr = strpbrk(ptr,"\\");
    *ptr = '\0'; ptr+=2;
    num = atoi(prev);//lock file here
    fwrite(ptr,sizeof(char),num,file);
    sendmessage("ACK",sock);//release lock here
    fclose(file);
  }else if(!strcmp(copy,"READ")){
    //read command
    ptr = strpbrk(ptr,"\\ ");
    *ptr = '\0'; ptr+=2;
    file = fopen(prev,"rb");
    if(!file){
      sendmessage("ERROR: file does not exist\n",sock);
      return;
    }
    fgets(readbuf,BUFFER_SIZE,file);
    num = strlen(readbuf);
    sprintf(temp,"ACK %d\n",num);
    sendmessage(temp,sock);
    sendmessage(readbuf,sock);
    fclose(file);
  }else if(!strcmp(copy,"LIST")){
    //List command
    dir = opendir(".");
    num = 0;
    while((dirfile = readdir( dir ) ) != NULL ){
      ++num;
    }
    closedir(dir);
    sprintf(temp,"%d\n",num);
    sendmessage(temp,sock);
    dir = opendir(".");
    while((dirfile = readdir( dir ) ) != NULL ){
      sprintf(temp,"%s\n",dirfile->d_name);
      sendmessage(temp,sock);
    }
    closedir(dir);
  }else if(!strcmp(copy,"DELETE")){
    //delete command
    ptr = strpbrk(ptr,"\\");
    *ptr = '\0'; ptr+=2;
    file = fopen(prev,"rb");
    if(!file){
      sendmessage("ERROR: file does not exist\n",sock);
      return;
    }
    fclose(file);
    remove(prev);
    sendmessage("ACK",sock);
  }else{
    //not a command
    sendmessage("ERROR: <no such command>\n",sock);
  }
}

void sendmessage(char * message, int sock){
  int n;
  n = send( sock, message, strlen(message), 0 );
  fflush( NULL );
  if ( n != strlen(message) ) {
    perror( "send() failed" );
  }
  return;
}
