
#define BUFFERSIZE 1024
#define MAXCLIENTS 20

int main(int argc, char ** argv){
  int port,sock,sockdescriptors[MAXCLIENTS];
  struct sockaddr_in server,client;
  char buffer[BUFFERSIZE];

  if(argv == 1){
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
  
  //set the socket structure
  server.sin_family = PF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(sock);
 
  //bind socket
  if(bind(sock,(struct sockaddr *)&server,sizeof(server)) < 0){
    perror( "bind() failed" );
    exit( EXIT_FAILURE );
  }
  
  //make a listener
  listen(sock,5);
  printf( "Listener bound to port %d\n", port );

  //server loop to run forever
  while(true){
    printf( "PARENT: Blocked on accept()\n" );
    sockdescriptors[0] = accept(sock,(struct sockaddr *)&client,
			 (socklen_t*)&(sizeof)client);
    printf( "PARENT: Accepted client connection\n" );
    
    
  }



  close(sock);
  return EXIT_SUCCESS;
}
