#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

int main(int argc, char **argv){
	bool encrypt;
	FILE * readf, * writef;
	char *filename, tmp[20], password[20], data[20];
	if(argc != 3){
		printf("error: need 2 arguments. Name of file and 0 for encryption\n");
		exit(EXIT_FAILURE);
	}
	encrypt = (atoi(argv[2])==0?1:0);
	strcpy(tmp,argv[1]);
	filename = strtok(tmp,".");
	if(encrypt){
		if(strcmp(strtok(NULL,"."),"txt")){
			printf("error: first argument is not a text file\n");
			exit(EXIT_FAILURE);
		}
		readf = fopen(argv[1],"r");
		if(!readf){
			printf("error: file does not exist\n");
			exit(EXIT_FAILURE);
		}
		writef = fopen(strcat(filename,".q"),"r");
		if(writef){
			printf("file %s already exist. continue y? \n",filename);
			scanf("%s",data);
			if(strcmp(data,"y")){
				exit(EXIT_SUCCESS);
			}
		}
		fclose(writef);
		writef = fopen(filename,"w");
		printf("password?\n");
		scanf("%s",password);
		if(strlen(password) > 20 || strlen(password) < 1 ){
			printf("error: password must be less than 20 char and not be empty\n");
			exit(EXIT_FAILURE);
		}
		
		//code to come
		
		fclose(readf);
		fclose(writef);
		printf("thank you for using this encryption software\n");
	}
	exit(EXIT_SUCCESS);
}

bool readFile(FILE * readf, char * data){
	
}