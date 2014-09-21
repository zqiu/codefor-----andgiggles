#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

int main(int argc, char **argv){
	int i,j,len,sub;
	char temp[100];
	strcpy(temp,argv[1]);
	len = strlen(temp);
	for(i = 0; i < 26; ++i){
		for(j = 0; j < len; ++j){
			if(temp[j] == ' '){continue;}
			temp[j] -= 1;
			if(temp[j] == '`'){
				temp[j] = 'z';
			}
		}
		printf("%s\n",temp);
	}
}

