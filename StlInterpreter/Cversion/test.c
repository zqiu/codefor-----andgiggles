#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

typedef unsigned char BYTE;
void read_file(FILE * file);
void write_file(FILE * file);

int main(int argc, char **argv){
	FILE * readf, *writef;
	BYTE * name;
	size_t numread,i;
	
	writef = fopen("hi.txt","w");
	write_file(writef);
	fclose(writef);
	
	readf = fopen("hi.txt","r");
	read_file(readf);
	fclose(readf);

	printf("opening in binary mode\n");
	readf = fopen("hi.txt","rb");
	read_file(readf);
	fclose(readf);
	
	remove("hi.txt");
	exit(EXIT_SUCCESS);
}

void write_file(FILE * file){
	int i;
	for(i = 0; i < 256; ++i){
		fputc((char)i,file);
	}
}

void read_file(FILE * file){
	int i,j;
	BYTE *buffer = (BYTE*)malloc(sizeof(BYTE));
	for(i = 0; i < 256; ++i){
		fseek(file,i,SEEK_SET);
		j = fread((void*)buffer,1,1,file);
		//printf("read in %x\n",buffer[0]);
		if(j == 0){
			printf("read file terminates on hex %d\n",i);
			clearerr(file);
		}
	}
	free(buffer);
}
