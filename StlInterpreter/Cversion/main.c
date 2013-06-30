#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

typedef unsigned char BYTE;
bool read(FILE * file, BYTE * name, unsigned long * numtriangles, float** normal, float** x, float** y, float** z);
bool getName(FILE * file, BYTE * buffer);
bool getNumTriangle(FILE * file, BYTE * buffer);
bool getNextTriangle(FILE * file, BYTE * buffer, BYTE * throwaway);

union{
	float f;
	BYTE b[sizeof(float)];
}sfloat;

int main(int argc, char **argv){
	FILE * readf;
	BYTE * name;
	unsigned long numfaces;
	float ** normal, **x, **y, **z;
	if(argc != 2){
		printf("error: need an argument: Name of file\n");
		exit(EXIT_FAILURE);
	}
	readf = fopen(argv[1],"r");
	if(!readf){
		printf("error: file does not exist\n");
		exit(EXIT_FAILURE);
	}
	read(readf,name,&numfaces,normal,x,y,z)?printf("read sucessfully\n"):printf("file to be read from is improperly formatted or cannot be read\n");
	fclose(readf);
	//printf("%f",normal[0][0]);
	exit(EXIT_SUCCESS);
}

bool read(FILE * file, BYTE * name, unsigned long * numtriangles, float** normal, float** x, float** y, float** z){
	unsigned long i;
	int j,k;
	BYTE * tempnumtriangle = (BYTE *) malloc(sizeof(BYTE) * 4);
	BYTE * temptrianglebuffer = (BYTE *) malloc(sizeof(BYTE) * 48);
	BYTE * throwaway = (BYTE *) malloc(sizeof(BYTE) * 2);
	
	name = (BYTE*) malloc(sizeof(BYTE) * 80);
	if(!getName(file,name)) return false;
	printf("name: %s \n",name);
	
	if(!getNumTriangle(file,tempnumtriangle)) return false;
	for(i = 0, *numtriangles = 0; i < 4; ++i){
		*numtriangles += (unsigned long) tempnumtriangle[i] << i;
	}
	printf("num of triangles: %ul \n",*numtriangles);
	if(*numtriangles >UINT_MAX){
		printf("note:number of triangles is greater than value of unsigned int. Program might not function correctly\n");
	}
	
	normal = (float **) malloc(sizeof(float*) * (unsigned int)(*numtriangles));
	x = (float **) malloc(sizeof(float*) * (unsigned int)(*numtriangles));
	y = (float **) malloc(sizeof(float*) * (unsigned int)(*numtriangles));
	z = (float **) malloc(sizeof(float*) * (unsigned int)(*numtriangles));
	for(i = 0; i < *numtriangles; ++i){
		normal[i] = (float *) malloc(sizeof(float) * 3);
		x[i] = (float *) malloc(sizeof(float) * 3);
		y[i] = (float *) malloc(sizeof(float) * 3);
		z[i] = (float *) malloc(sizeof(float) * 3);
	}
	
	for(i = 0; i < *numtriangles; ++i){
		if(!getNextTriangle(file,temptrianglebuffer,throwaway))return false;
		for(k = 0; k < 4; ++k){
			for(j = 0; j < 3; ++j){
				sfloat.b[0] = k*12 + j*4;
				sfloat.b[1] = k*12 + j*4 + 1;
				sfloat.b[2] = k*12 + j*4 + 2;
				sfloat.b[3] = k*12 + j*4 + 3;
				switch(k){
					case 0:
						normal[i][j] = sfloat.f;
						break;
					case 1:
						x[i][j] = sfloat.f;
						break;
					case 2:
						y[i][j] = sfloat.f;
						break;
					default:
						z[i][j] = sfloat.f;
						break;
				}
			}
		}
	}
	return true;
}

bool getName(FILE * file, BYTE * buffer){
	size_t numread = fread((void*)buffer,1,80,file);
	return (numread == 80)?true:false;
}

bool getNumTriangle(FILE * file, BYTE * buffer){
	size_t numread = fread((void*)buffer,1,4,file);
	return (numread == 4)?true:false;
}

bool getNextTriangle(FILE * file, BYTE * buffer, BYTE * throwaway){
	size_t numread = fread((void*)buffer,1,48,file);
	if(numread != 48) return false;
	numread = fread((void*)throwaway,1,2,file);
	return (numread == 2)?true:false;
}
