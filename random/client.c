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

int main()
{
  /* create TCP client socket (endpoint) */
  int sock = socket( PF_INET, SOCK_STREAM, 0 );

  if ( sock < 0 )
  {
    perror( "socket() failed" );
    exit( EXIT_FAILURE );
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl((((((127 << 8) | 0) << 8) | 0) << 8) | 1);
  unsigned short port = 8200;
  server.sin_port = htons( port );

  printf( "server address is %s\n", inet_ntoa( server.sin_addr ) );

  if ( connect( sock, (struct sockaddr *)&server,
                sizeof( server ) ) < 0 )
  {
    perror( "connect() failed" );
    exit( EXIT_FAILURE );
  }

  char * msg = "hello world";
  int n = write( sock, msg, strlen( msg ) );
fflush( NULL );
  if ( n < strlen( msg ) )
  {
    perror( "write() failed" );
    exit( EXIT_FAILURE );
  }

  char buffer[ BUFFER_SIZE ];
  n = read( sock, buffer, BUFFER_SIZE );  // BLOCK
  if ( n < 0 )
  {
    perror( "read() failed" );
    exit( EXIT_FAILURE );
  }
  else
  {
    buffer[n] = '\0';
    printf( "Received message from server: %s\n", buffer );
  }

  close( sock );

  return EXIT_SUCCESS;
}
