#include <stdlib.h>

int main(int argc, char ** argv){
	FILE * readf, *writef;
	readf = fopen("toread.txt","rb");
	if(!readf){
		printf("error: file does not exist\n");
		exit(EXIT_FAILURE);
	}
}