#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

void encodeFile(FILE * readf, FILE * writef, char * pass, char logging);
void decodeFile(FILE * readf, FILE * writef, char * pass, char logging);

int main(int argc, char **argv){
	bool encrypt;
	FILE * readf, * writef;
	char *filename, tmp[20], password[20], data[20];
	if(argc < 3){
		printf("error: need 2 arguments. Name of file and 0 for encryption\n");
		exit(EXIT_FAILURE);
	}
	encrypt = (atoi(argv[2])==0?1:0);
	strcpy(tmp,argv[1]);
	filename = strtok(tmp,".");
	if(!encrypt){
		if(strcmp(strtok(NULL,"."),"q")){
			printf("error: first argument is not a q file\n");
			exit(EXIT_FAILURE);
		}
	}
	readf = fopen(argv[1],"rb");
	if(!readf){
		printf("error: file does not exist\n");
		exit(EXIT_FAILURE);
	}
	if(encrypt){
		writef = fopen(strcat(filename,".q"),"r");
	}else{
		writef = fopen(strcat(filename,".dec"),"r");
	}
	if(writef){
		printf("file %s already exist. continue y? \n",filename);
		scanf("%s",data);
		if(strcmp(data,"y")){
			exit(EXIT_SUCCESS);
		}
	}
	fclose(writef);
	writef = fopen(filename,"wb");
	printf("password?\n");
	scanf("%s",password);
	if(strlen(password) > 20 || strlen(password) < 1 ){
		printf("error: password must be less than 20 char and not be empty\n");
		exit(EXIT_FAILURE);
	}		
	if(encrypt){
		encodeFile(readf,writef,password,(argc == 4)?(char)!strcmp("--debug",argv[3]):0);
	}else{
		decodeFile(readf,writef,password,(argc == 4)?(char)!strcmp("--debug",argv[3]):0);		
	}
	fclose(readf);
	fclose(writef);
	exit(EXIT_SUCCESS);
}

void encodeFile(FILE * readf, FILE * writef, char * pass,char logging){
	int i,passsum = 0,read = 0,length = strlen(pass),toput;
	char buf[10];
	FILE * log = NULL;
	if(logging){
		log = fopen("log.txt","w");
	}
	for(i = 0; i < length; ++i){
		passsum += pass[i];
	}
	i = 0;
	while(read != EOF){
		read = fgetc(readf);
		if(read == -1){
			break;
		}
		sprintf(buf,"%d\n",read);
		if(log){
			fwrite(buf, sizeof(char), strlen(buf), log);
		}
		toput = (read - pass[i%length] + i%passsum + 256)%256;
		fputc(toput,writef);
		i++;
	}
	fclose(log);
}

void decodeFile(FILE * readf, FILE * writef, char * pass,char logging){
	int i,passsum = 0,read = 0,length = strlen(pass),toput;
	char buf[10];
	FILE * log = NULL;
	if(logging){
		log = fopen("log.txt","w");
	}
	for(i = 0; i < length; ++i){
		passsum += pass[i];
	}
	i = 0;
	while(read != EOF){
		read = fgetc(readf);
		if(read == -1){
			break;
		}
		sprintf(buf,"%d\n",read);
		if(log){
			fwrite(buf, sizeof(char), strlen(buf), log);
		}
		toput = (read + (pass[i % length] - i%passsum%256 + 256))%256;
		fputc(toput,writef);
		i++;
	}
	fclose(log);
}