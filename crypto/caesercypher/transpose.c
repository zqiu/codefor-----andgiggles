#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

int main(int argc, char **argv){
	int i,j,len,sub = 0;
	char temp[] = "Ieora  u. mudsfw h l iorwowdicroipi f  nletb cygl heyro\" y  oyu.gotfupr euha t Ft iihoeo dsra.frao lv f nncyeToyo'h  hrottasdetuh lto srehlrnm  raeaeaie vni gsfceggli fh ehoc\"oaa.tt nrl   spotlpIf htsertoor ,no rfan gbsw soyelhabeto!";
	
	//strcpy(temp,argv[1]);
	len = strlen(temp);
	for(i = 2; i < len/2; ++i){
		for(j = 0; j < len; ++j){
			printf("%c",temp[sub]);
			sub += i;
			if(sub >= len){
				sub = sub - len;
			}
		}
		sub = 0;
		printf("%d\n\n",i);
	}
}