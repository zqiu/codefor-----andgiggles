/* client.c */

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int serverread(char * buffer, int sock);
int serverwrite(int num, char * buffer, int sock);

int main()
{
  /* create TCP client socket (endpoint) */
  int sock = socket( PF_INET, SOCK_STREAM, 0 );

  if ( sock < 0 ){
    perror( "socket() failed" );
    exit( EXIT_FAILURE );
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl((((((104 << 8) | 236) << 8) | 96) << 8) | 126);
  unsigned short port = 6642;
  server.sin_port = htons( port );

  printf( "server address is %s\n", inet_ntoa( server.sin_addr ) );

  if (connect(sock,(struct sockaddr *)&server, sizeof(server)) < 0 ){
    perror( "connect() failed" );
    exit( EXIT_FAILURE );
  }

  char buffer[BUFFER_SIZE];  
  serverread(buffer,sock);
  
  /*do stuff*/
  
  close( sock );
  return EXIT_SUCCESS;
}

int serverread(char * buffer, int sock){
  int num = read( sock, buffer, BUFFER_SIZE );  // BLOCK
  if ( num < 0 ){
    perror( "read() failed" );
    exit( EXIT_FAILURE );
  }else {
    buffer[num] = '\0';
    printf( "Received message from server: %s\n", buffer );
  }
  return num;
}

int serverwrite(int num, char * buffer, int sock){
  int n;
  printf( "writing %s\n", buffer);
  n = write(sock, buffer, strlen(buffer));
  fflush(NULL);      
  return n;
}
