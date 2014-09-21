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
	sub = temp[0] - 't';
	//for(i = 0; i < 26; ++i){
		for(j = 0; j < len; ++j){
			if(temp[j] == ' '){continue;}
			if(temp[j] > '\x6E') {temp[j] -= 26;}
			temp[j] -= sub;
			if(temp[j] < 'a'){
				temp[j] += 26;
			}else if(temp[j] > 'z'){
				temp[j] -= 26;
			}
		}
		printf("%d,%s\n",sub,temp);
	//}
}
