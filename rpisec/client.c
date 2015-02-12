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

#define BUFFER_SIZE 5000

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
  server.sin_addr.s_addr = htonl((((((54 << 8) | 164) << 8) | 0) << 8) | 57);
  unsigned short port = 2211;
  server.sin_port = htons( port );

  printf( "server address is %s\n", inet_ntoa( server.sin_addr ) );

  if ( connect( sock, (struct sockaddr *)&server,
                sizeof( server ) ) < 0 )
  {
    perror( "connect() failed" );
    exit( EXIT_FAILURE );
  }


  int n;
  int count = 0;
  char buffer[ BUFFER_SIZE ], *ptr, *my, *temp, first, initial = 1;
  int num,i,spacecount;
  
  while((n = read( sock, buffer, BUFFER_SIZE ))){  // BLOCK
    if ( n < 0 ) {
      perror( "read() failed" );
      exit( EXIT_FAILURE );
    } else {
      buffer[n] = '\0';
      printf( "Received message from server: %s\n", buffer );
      if(initial){
	ptr = strpbrk(buffer,"G");
	if(!ptr){
	  continue;
	}
	initial = 0;
      
	ptr = strpbrk(ptr,"R");
	ptr += 11;
      }else{
	ptr = buffer+11;
      }
      if(*ptr == 'f'){
	first = 1;
      }else{
	first = 0;
      }
      ptr = strpbrk(ptr,"1234567890");
      num = atoi(ptr);
      ptr = strpbrk(ptr,":");
      ptr += 2;
      my = ptr;
      printf("returned:%d,%s",num,ptr);	
	
      if(first){
	for(i = 0; i < num; ++i){
	  my = strpbrk(my," ");
	  if(!my){
	    break;
	  }
	  my++;
	}
	if(my){
	  my--;
	  *my = '\n';
	  *(my+1) = '\0';
	}
	write(sock,ptr,strlen(ptr));
	printf("returned:%s",ptr);
      }else{
	spacecount = 0;
	temp = my;
	while(temp){
	  //printf("sapacecount%d\n",spacecount);
	  temp = strpbrk(temp," ");
	  if(!temp){
	    break;
	  }
	  temp++;
	  spacecount++;
	}
	printf("sapacecount%d\n",spacecount);
	for(i = 0; i < (spacecount-num+1); ++i){
	  my = strpbrk(my," ");
	  my++;	  
	  //printf("%d,%s\n",i,my);
	}
	write(sock,my,strlen(my));
	printf("returne:%s\n",my);
      }
      printf("returned:%s\n",ptr);
    }
  }

  /*
  char * msg = "ADD me.txt 20\\nhello wo\0x44rld";
  int n = write( sock, msg, strlen( msg ) );
  fflush( NULL );
  if ( n < strlen( msg ) ){
    perror( "write() failed" );
    exit( EXIT_FAILURE );
  }
  */
  
  close( sock );

  return EXIT_SUCCESS;
}
