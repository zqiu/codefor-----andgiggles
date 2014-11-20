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
void sendmessage(char * message, int sock, int length);
void interpret_buffer(char * buf, int sock, int n);

pthread_mutex_t lock;

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
  
  //set up mutex
  if (pthread_mutex_init(&lock, NULL) != 0){
    printf("\n mutex init failed\n");
    return 1;
  }

  //make a listener
  listen(sock,MAXCLIENTS);
#ifdef DEBUG
  printf( "Listener bound to port %d on addr %s\n", port , inet_ntoa( (struct in_addr)server.sin_addr ));
#endif

  //server loop to run forever
  while(true){
#ifdef DEBUG
    printf( "PARENT: Blocked on accept()\n" );
#endif
    clientlen = sizeof(client);
    sockdescriptors = accept(sock,(struct sockaddr *)&client,
			     (socklen_t*)&clientlen);
#ifdef DEBUG
    printf( "PARENT: Accepted client connection\n" );
#endif
    printf( "recieved incoming connection from %s\n",inet_ntoa(client.sin_addr));
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
#ifdef DEBUG
    printf("%d",currentconnections);
#endif
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
      printf( "[thread %u]Client closed its socket...terminating\n", (unsigned int)pthread_self());
    } else {
      buffer[n] = '\0';  /* terminate buffer */
      printf( "[thread %u]Rcvd:%s\n",(unsigned int)pthread_self(),buffer );
      interpret_buffer(buffer,csock,n);      
    }
  }while(n > 0);
  close(csock);
  free(arg);
  return NULL;
}

void interpret_buffer(char * buf, int sock, int n){
  char copy[BUFFER_SIZE], *ptr, *prev, readbuf[BUFFER_SIZE],  msg[BUFFER_SIZE];
  FILE * file;
  int num;
  DIR * dir;
  struct dirent * dirfile;
  struct stat fileentry;

  memcpy(copy,buf,n+1);
  ptr = strpbrk(copy," \\");
  if(!ptr){
    strcpy(msg,"ERROR: <no such command>\n");
    sendmessage(msg,sock,strlen(msg));
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
      strcpy(msg,"ERROR: file exist\n");
      sendmessage(msg,sock,strlen(msg));
      return;
    }
    file = fopen(prev,"wb");
    printf("%s\n",prev);
    
    prev = ptr; ptr = strpbrk(ptr,"\\"); 
    *ptr = '\0'; ptr+=2;
    num = atoi(prev);//lock both read and write lock
    pthread_mutex_lock(&lock);
    fwrite(ptr,sizeof(char),num,file);
    pthread_mutex_unlock(&lock);
    strcpy(msg,"ACK\n");//release read and write lock 
    sendmessage(msg,sock,strlen(msg));
    fclose(file);
  }else if(!strcmp(copy,"APPEND")){
    //append command
    ptr = strpbrk(ptr," ");
    *ptr = '\0'; ++ptr;
    file = fopen(prev,"rb");
    if(!file){
      strcpy(msg,"ERROR: file does not exist\n");
      sendmessage(msg,sock,strlen(msg));
      return;
    }
    fclose(file);
    file = fopen(prev,"ab");
    prev = ptr; ptr = strpbrk(ptr,"\\");
    *ptr = '\0'; ptr+=2;
    num = atoi(prev);//wait until file is unlocked then lock file here
    pthread_mutex_lock(&lock);
    fwrite(ptr,sizeof(char),num,file);
    pthread_mutex_unlock(&lock);
    strcpy(msg,"ACK\n");//release lock here
    sendmessage(msg,sock,strlen(msg));
    fclose(file);
  }else if(!strcmp(copy,"READ")){
    //read command
    ptr = strpbrk(ptr,"\\ ");
    *ptr = '\0'; ptr+=2;//wait until the file is not locked
    file = fopen(prev,"rb");
    if(!file){
      strcpy(msg,"ERROR: file does not exist\n");
      sendmessage(msg,sock,strlen(msg));
      return;
    }
    pthread_mutex_lock(&lock);
    num = fread(readbuf,sizeof(char),BUFFER_SIZE,file);
    pthread_mutex_unlock(&lock);    
    sprintf(msg,"ACK %d\n",num);
    sendmessage(msg,sock,strlen(msg));
    strcat(readbuf,"\n");
    sendmessage(readbuf,sock,num + 1);
    fclose(file);
  }else if(!strcmp(copy,"LIST")){
    //List command
    dir = opendir(".");
    num = 0;
    while((dirfile = readdir( dir ) ) != NULL ){
      lstat(dirfile->d_name,&fileentry);
      if(S_ISREG(fileentry.st_mode)){
	++num;
      }
    }
    closedir(dir);
    sprintf(msg,"%d\n",num);
    sendmessage(msg,sock,strlen(msg));
    dir = opendir(".");
    while((dirfile = readdir( dir ) ) != NULL ){
      lstat(dirfile->d_name,&fileentry);
      if(S_ISREG(fileentry.st_mode)){
	sprintf(msg,"%s\n",dirfile->d_name);
	sendmessage(msg,sock,strlen(msg));
      }
    }
    closedir(dir);
  }else if(!strcmp(copy,"DELETE")){
    //delete command
    ptr = strpbrk(ptr,"\\");
    *ptr = '\0'; ptr+=2;
    file = fopen(prev,"rb");
    if(!file){
      strcpy(msg,"ERROR: file does not exist\n");
      sendmessage(msg,sock,strlen(msg));
      return;
    }
    fclose(file);
    pthread_mutex_lock(&lock);
    remove(prev);
    pthread_mutex_unlock(&lock);
    strcpy(msg,"ACK\n");
    sendmessage(msg,sock,strlen(msg));
  }else{
    //not a command
    strcpy(msg,"ERROR: <no such command>\n");
    sendmessage(msg,sock,strlen(msg));
  }
}

void sendmessage(char * message, int sock,int length){
  int i;
  send( sock, message, length, 0 );
  fflush( NULL );
#ifdef DEBUG
  printf("length = %d\n",length);
#endif
  printf("[thread %u]sent:",(unsigned int)pthread_self());
  for(i = 0; i < length; ++i){
    printf("%c",message[i]);
  }
  printf("\n");
  return;
}
