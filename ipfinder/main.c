#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void processrequest(int num, int timeout,char * buffer);

int main(int argc, char ** argv){
	char * buffer = (char*)malloc(sizeof(char)*50);
	int timeout,num;
	if(argc != 3){
		free(buffer);
		printf("arg 1:num changable address, arg 2:timeout");
		return 1;
	}
	timeout = atoi(argv[2]);
	num = atoi(argv[1]);
	if(num < 1 || num > 4){
			free(buffer);
			printf("arg 1 between 1 and 4");
			return 1;
	}
	processrequest(num,timeout,buffer);
	free(buffer);
	printf("\nsucess");
	return 0;
}

void processrequest(int num,int timeout, char * buffer){
	int i,j,k,l;
	for(i = 0; i < 255; ++i){
		if(num < 4){
			i = 192;
		}
		for(j = 0; j < 255; ++j){
			if(num < 3){
				j = 168;
			}
			for(k = 0; k < 255; ++k){
				if(num < 2){
					k = 1;
				}
				for(l = 0; l < 255; ++l){
					sprintf(buffer,"ping %d.%d.%d.%d -w %d >> data.txt",i,j,k,l,timeout);
					printf("%s\n",buffer);
					system(buffer);
				}
				if(num < 2){
					break;
				}
			}
			if(num < 3){
				break;
			}
		}
		if(num < 4){
			break;
		}
	}
}