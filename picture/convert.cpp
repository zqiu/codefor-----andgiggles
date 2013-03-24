#include "random.h"
#include <cstdlib>
#include <cstring>

int main(int argc, char ** argv){
	if(argc != 3){
		std::cerr << "wring number of args, starting num, next num";
	}
	int arg = atoi(argv[1]), arg2 = atoi(argv[2]),i;
	for(i = 0; i < arg2; ++i){
		char filename[15],ppm[15],png[15],temp[15],command[60];
		strcpy(filename,"pic");
		sprintf(temp,"%d",atoi(argv[1])+ i);
		strcat(filename,temp);
		strcpy(ppm,filename);
		strcpy(png,filename);
		strcat(ppm,".png");		
		strcat(png,".jpg");
		strcpy(command, "convert ");
		strcat(command, ppm);
		strcat(command, " ");
		strcat(command, png);
		strcat(command, "\r");
		std::cout << command << "\n";
		system(command);
	}
}