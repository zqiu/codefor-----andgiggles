#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

typedef unsigned char BYTE;
bool read(FILE * file, BYTE ** name, unsigned long * numtriangles, float*** normal, float*** v1, float*** v2, float*** v3);
bool getName(FILE * file, BYTE * buffer);
bool getNumTriangle(FILE * file, BYTE * buffer);
bool getNextTriangle(FILE * file, BYTE * buffer, BYTE * throwaway);
void printOutArray(float *** array, unsigned long length, int depth);
void writefile(FILE * file,BYTE ** name, unsigned long * numtriangles, float*** normal, float*** v1, float*** v2, float*** v3);
float *** allpoints(unsigned long * numtriangles, float*** v1, float*** v2, float*** v3);
bool comparepoints(float ** p1, float **p2);
bool inSet(float *** set, unsigned long * size, float ** p);

union{
	float f;
	BYTE b[sizeof(float)];
}sfloat;

int main(int argc, char **argv){
	FILE * readf, *writef;
	char *filename,*data, *tmp;
	BYTE * name;
	unsigned long numfaces;
	float ** normal, **v1, **v2, **v3;
	if(argc != 2){
		printf("error: need an argument: Name of file\n");
		exit(EXIT_FAILURE);
	}
	strcpy(tmp,argv[1]);
	filename = strtok(tmp,".");
	if(!strcmp(strtok(NULL,"."),"STL\n")){
			printf("error: first argument is not a stl file\n");
			exit(EXIT_FAILURE);
	}
	readf = fopen(argv[1],"r");
	if(!readf){
		printf("error: file does not exist\n");
		exit(EXIT_FAILURE);
	}
	writef = fopen(strcat(filename,".ttl\n"),"r");
	if(writef){
		printf("file %s already exist. continue y? \n",filename);
		scanf("%c",data);
		if(!strcmp(data,"y")){
			exit(EXIT_SUCCESS);
		}
	}
	fclose(writef);
	writef = fopen(filename,"w");
	read(readf,&name,&numfaces,&normal,&v1,&v2,&v3)?printf("read sucessfully\n"):printf("file to be read from is improperly formatted or cannot be read\n");
	fclose(readf);
	fclose(writef);
	printOutArray(&normal,numfaces,3);
	printOutArray(&v1,numfaces,3);
	printOutArray(&v2,numfaces,3);
	printOutArray(&v3,numfaces,3);
	exit(EXIT_SUCCESS);
}

void writefile(FILE * file, BYTE ** name, unsigned long * numtriangles, float*** normal, float*** v1, float*** v2, float*** v3){
	fputc('#',file);
	fputs(*name,file);
	fputs("\n@base <http://example.com/>.\n@prefix xsd: <http://www.w3.org/2001/XMLSchema/>.\n",file);
	
}

float *** allpoints(unsigned long * numtriangles, float*** v1, float*** v2, float*** v3){
	unsigned long numpoints = 0,i,j,maxpoints = *numtriangles + 2;
	float ** ans = (float**)malloc(sizeof(float*) * maxpoints);
	for(i = 0; i < numpoints; ++i){
		ans[i] = (float*)malloc(sizeof(float)*3);
	}
	for(i = 0; i < maxpoints; ++i){
		if(!inSet(&ans,&maxpoints,&((*v1)[i]))){
			ans[numpoints][0] = (*v1)[i][0];
			ans[numpoints][1] = (*v1)[i][1];
			ans[numpoints][2] = (*v1)[i][2];
			numpoints++;
		}
		if(!comparepoints(&((*v1)[i]),&((*v2)[i]))){
			if(!inSet(&ans,&maxpoints,&((*v2)[i]))){
				ans[numpoints][0] = (*v2)[i][0];
				ans[numpoints][1] = (*v2)[i][1];
				ans[numpoints][2] = (*v2)[i][2];
				numpoints++;
			}
		}
		if(!comparepoints(&((*v1)[i]),&((*v3)[i])) && !comparepoints(&((*v1)[i]),&((*v3)[i]))){
			if(!inSet(&ans,&maxpoints,&((*v2)[i]))){
				ans[numpoints][0] = (*v3)[i][0];
				ans[numpoints][1] = (*v3)[i][1];
				ans[numpoints][2] = (*v3)[i][2];
				numpoints++;
			}
		}
	}
	return &ans;
}

bool comparepoints(float ** p1, float **p2){
	return (*p1)[0] == (*p2)[0] && (*p1)[1] == (*p2)[1] && (*p1)[2] == (*p2)[2];
}

bool inSet(float *** set, unsigned long * size, float ** p){
	unsigned long i;
	for(i = 0; i < *size; ++i){
		if(comparepoints(&((*set)[i]),p)) return true;
	}
	return false;
}

bool read(FILE * file, BYTE ** name, unsigned long * numtriangles, float*** normal, float*** v1, float*** v2, float*** v3){
	unsigned long i;
	int j,k;
	BYTE * tempnumtriangle = (BYTE *) malloc(sizeof(BYTE) * 4);
	BYTE * temptrianglebuffer = (BYTE *) malloc(sizeof(BYTE) * 48);
	BYTE * throwaway = (BYTE *) malloc(sizeof(BYTE) * 2);
	
	*name = (BYTE*) malloc(sizeof(BYTE) * 81);
	if(!getName(file,*name)) return false;
	(*name)[80] = '\0';
	printf("name: %s \n",*name);
	
	if(!getNumTriangle(file,tempnumtriangle)) return false;
	for(i = 0, *numtriangles = 0; i < 4; ++i){
		*numtriangles += (unsigned long) tempnumtriangle[i] << i;
	}
	printf("num of triangles: %lu \n",*numtriangles);
	if(*numtriangles >UINT_MAX){
		printf("note:number of triangles is greater than value of unsigned int. Program might not function correctly\n");
	}
	
	*normal = (float **) malloc(sizeof(float*) * (unsigned int)(*numtriangles));
	*v1 = (float **) malloc(sizeof(float*) * (unsigned int)(*numtriangles));
	*v2 = (float **) malloc(sizeof(float*) * (unsigned int)(*numtriangles));
	*v3 = (float **) malloc(sizeof(float*) * (unsigned int)(*numtriangles));
	for(i = 0; i < *numtriangles; ++i){
		(*normal)[i] = (float *) malloc(sizeof(float) * 3);
		(*v1)[i] = (float *) malloc(sizeof(float) * 3);
		(*v2)[i] = (float *) malloc(sizeof(float) * 3);
		(*v3)[i] = (float *) malloc(sizeof(float) * 3);
	}
	
	for(i = 0; i < *numtriangles; ++i){
		if(!getNextTriangle(file,temptrianglebuffer,throwaway))return false;
		for(k = 0; k < 4; ++k){
			for(j = 0; j < 3; ++j){
				sfloat.b[0] = temptrianglebuffer[k*12 + j*4];
				sfloat.b[1] = temptrianglebuffer[k*12 + j*4 + 1];
				sfloat.b[2] = temptrianglebuffer[k*12 + j*4 + 2];
				sfloat.b[3] = temptrianglebuffer[k*12 + j*4 + 3];
				switch(k){
					case 0:
						(*normal)[i][j] = sfloat.f;
						break;
					case 1:
						(*v1)[i][j] = sfloat.f;
						break;
					case 2:
						(*v2)[i][j] = sfloat.f;
						break;
					default:
						(*v3)[i][j] = sfloat.f;
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

void printOutArray(float *** array, unsigned long length, int depth){
	unsigned long i;
	int j;
	for(i = 0; i < length; ++i){
		for(j = 0; j < depth; ++j){
			printf("%lu,%d:%f \n",i,j,(*array)[i][j]);
		}
		printf("\n");
	}
}