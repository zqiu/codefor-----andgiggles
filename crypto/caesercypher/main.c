#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

int main(int argc, char **argv){
	int i,j,len;
	char temp[100];
	strcpy(temp,argv[1]);
	len = strlen(temp);
	for(i = 0; i < 26; ++i){
		for(j = 0; j < len; ++j){
			temp[j] -= 1;
		}
		printf("%d:%s\n",i,temp);
	}
}