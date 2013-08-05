#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

typedef unsigned char BYTE;
bool read(FILE * file, BYTE ** name, unsigned long * numtriangles, float*** normal, float*** v1, float*** v2, float*** v3, bool debug);
bool getName(FILE * file, BYTE ** buffer);
bool getNumTriangle(FILE * file, BYTE ** buffer);
bool getNextTriangle(FILE * file, BYTE ** buffer, BYTE ** throwaway);
void printOutArray(float *** array, unsigned long length, int depth);
void writeFile(FILE * file,BYTE ** name, unsigned long * numtriangles, float*** normal, float*** v1, float*** v2, float*** v3, bool debug);
unsigned long allPoints(unsigned long * numtriangles, float*** v1, float*** v2, float*** v3,float*** writeto);
bool comparePoints(float ** p1, float **p2);
bool inSet(float *** set, unsigned long * size, float ** p);

//structure that converts bytes to floats and back
union{
	float f;
	BYTE b[sizeof(float)];
}sfloat;

int main(int argc, char **argv){
	FILE * readf, *writef;
	char cpy[81], *filename, *extension = NULL, *next, data;
	BYTE * name;
	unsigned long numfaces,i;
	float ** normal, **v1, **v2, **v3;
	bool debug = false, readsucessful;
	//reading in file arguments giving an error if number of file arguments does not match
	if(argc != 2 && argc != 3){
		printf("error: need an argument: Name of file\n");
		exit(EXIT_FAILURE);
	}
	//reading in if the user wants the program in debug mode
	if(argc == 3){
		debug = true;
	}
	//opening the file and throwing an error if file does not exist
	readf = fopen(argv[1],"rb");
	if(!readf){
		printf("error: file does not exist\n");
		exit(EXIT_FAILURE);
	}
	//getting extension and name of fil so that program can check if file is an stl file
	strncpy(cpy,argv[1],80);
	next = strtok(cpy,"./");
	while(next){
		filename = extension;
		extension = next;
		next = strtok(NULL,"./");
	}
	if(!extension || strcmp(extension,"STL")){
			printf("error: first argument is not a stl file\n");
			exit(EXIT_FAILURE);
	}
	//opening a file with the exact same file name just with the ttl extension. If file exists already program will ask if the user would like to continue
	writef = fopen(strcat(filename,".n3"),"r");
	if(writef){
		printf("file %s already exist. continue y? \n",filename);
		scanf("%s",&data);
		if(data != 'y'){
			exit(EXIT_SUCCESS);
		}
	}
	fclose(writef);
	writef = fopen(filename,"w");
	if(debug){
		printf("writing file to %s\n",filename);
	}
	//program will now try to read in values from the file and store them into their associated variables
	readsucessful = read(readf,&name,&numfaces,&normal,&v1,&v2,&v3,debug);
	readsucessful?printf("read sucessfully\n"):printf("file to be read from is improperly formatted or cannot be read\n");
	//if program is sucessful in reading, will now try to write the file in ttl format
	if(readsucessful){
		writeFile(writef,&name,&numfaces,&normal,&v1,&v2,&v3,debug);
		printf("writing sucessful\n");
	}
	//if not sucessful will print our debuggin statements if debugging is on
	if(!readsucessful) {
		printf("write unsucessful\n");		
		if(debug){
			perror("The following error occured");
			printf("end of file?: %s\n",feof(readf)?"yes":"no");
			printf("file error?: %s\n",ferror(readf)?"yes":"no");
		}else{
			printf("run program again with a 2nd argument to enter debug mode\n");
		}
	}
	//if debuggin is on will prompt user on whether they would like to print out the read data
	if(debug){
		printf("print normals? y? \n");
		scanf("%s",&data);
		if(data == 'y'){
			printOutArray(&normal,numfaces,3);
		}
		printf("print V1? y? \n");
		scanf("%s",&data);
		if(data == 'y'){
				printOutArray(&v1,numfaces,3);
		}
		printf("print V2? y? \n");
		scanf("%s",&data);
		if(data == 'y'){
			printOutArray(&v2,numfaces,3);
		}
		printf("print V3? y? \n");
		scanf("%s",&data);
		if(data == 'y'){
				printOutArray(&v3,numfaces,3);
		}
	}
	//closing the file streams and freeing memory
	fclose(readf);
	fclose(writef);
	for(i = 0; i < numfaces; ++i){
		free(normal[i]);
		free(v1[i]);
		free(v2[i]);
		free(v3[i]);
	}
	free(normal);
	free(v1);
	free(v2);
	free(v3);
	free(name);
	exit(EXIT_SUCCESS);
}

/*arguments:
	file pointer of file to be written to
	pointer to array of BYTES of the name of the file
	pointer to an unsigned long of the number of triangle in the file 
	4 pointer to 2d arrays of floats representing the coordinates 
	bool designating whether to be in debug modeor not
  return:NULL
*/
void writeFile(FILE * file, BYTE ** name, unsigned long * numtriangles, float*** normal, float*** v1, float*** v2, float*** v3, bool debug){
	float** points, *tmp, percent = 0.0;
	char buffer[20];
	unsigned long numpoints,i,j,p1 = ULONG_MAX,p2 = ULONG_MAX,p3 = ULONG_MAX;
	FILE * temp;
	//writes header for the ttl file
	fputs("thank you for using this program. This was written by Ze Qin Qiu. You can contact him at zqiu1994@gmail.com",file);
	//finds out all the points in the file and prints out num points if in debug mode
	numpoints = allPoints(numtriangles,v1,v2,v3,&points);
	if(debug){
		printf("there are %lu points\n",numpoints);
	}
	system("mkdir files");
	system("cd files");
	temp = fopen("folder.n3","w");
	writeheading(temp,10000,name,"<http://jazz.net/ns/dm/folder#Folder>");
	fclose(temp);
	//for every point writes the ttl format for it. Will also print in increments of 15% completion
	for(i = 0; i < numpoints; ++i){
		sprintf(buffer,"%s%lu","point",i);
		temp = fopen(buffer,"w");
		writePoint(temp,buffer,i+10001,10000,&(points[i]));
		if(i*100/numpoints > percent + 15){
			percent = ((i*100/numpoints)/5)*5;
			printf("%.0f%% of points written\n",percent);
		}
		fclose(temp);
	}
	printf("finished writing all the points\n");
	//for every triangle writes the ttl format for it. Will also print in increments of 15% completion
	percent = 0.0;
	for(i = 0; i < *numtriangles;++i){
		sprintf(buffer,"%s%lu","triangle",i);
		temp = fopen(buffer,"w");
		for(j = 0; j < numpoints; ++j){
			tmp = points[j];
			if(comparePoints(&tmp,&((*v1)[i]))){
				p1 = j;
			}
			if(comparePoints(&tmp,&((*v2)[i]))){
				p2 = j;
			}
			if(comparePoints(&tmp,&((*v3)[i]))){
				p3 = j;
			}
		}
		writetriangle(temp,buffer,i+10001+numpoints,&((*normal)[i]),p1,p2,p3);
		if(i*100/(*numtriangles) > percent + 15){
			percent = ((i*100/(*numtriangles))/5)*5;
			printf("%.0f%% of triangles written\n",percent);
		}
		fclose(temp);
	}
	//freeing the data stored into the memory allocated to track all the poinsts
	for(i = 0; i < *numtriangles + 2; ++i){
		free(points[i]);
	}
	free(points);
}
void writeheading(FILE* file,unsigned long id, char * name,char * type){
	char idtitle[50];
	sprintf(idtitle,"%s%lu%s","<https://ssejtsserver:9443/dm/models/",id,"> ");
	
	fputs(idtitle,file);
	fputs("<http://purl.org/dc/terms/title> \"",file);
	fputs(name,file);
	fputs("\"^^<http://www.w3.org/2001/XMLSchema#string> .\n",file);
	
	fputs(idtitle,file);
	fputs("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://jazz.net/ns/dm/core#Document> .\n",file);
	
	fputs(idtitle,file);
	fputs("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type> ",file);
	fputs(type,file);
	fputs(" .\n",file);
	
	fputs(idtitle,file);
	fputs("<http://www.w3.org/2000/01/rdf-schema#label> \"",file);
	fputs(name,file);
	fputs("\" .\n",file);
	
	fputs(idtitle,file);
	fputs("<http://xmlns.com/foaf/0.1/page> ",file);
	fputs(idtitle,file);
	fputs(" .\n",file);
}
void writetriangle(FILE* file, char * name, unsigned long id, unsigned long folderid, float ** point, unsigned long p1, unsigned long p2, unsigned long p3){
	char idtitle[50],temp[10];
	sprintf(idtitle,"%s%lu%s","<https://ssejtsserver:9443/dm/models/",id,"> ");
	writeheading(file,id,name,"<https://ssejtsserver:9443/dm/models/stlinterpreter#Triangle>");
	
	printXYZ(file,point,id);
	
	fputs(idtitle,file);
	fputs("<https://ssejtsserver:9443/dm/models/stlinterpreter#Points> _:p1 .\n",file);
	
	fputs("_:p1 <http://www.w3.org/1999/02/22-rdf-syntax-ns#first> ",file);
	sprintf(idtitle,"%s%lu%s","<https://ssejtsserver:9443/dm/models/",p1,"> ");
	fputs(idtitle,file);
	fputs(" .\n",file);
	
	fputs("_:p1 <http://www.w3.org/1999/02/22-rdf-syntax-ns#rest> _:p2 .\n",file);
	
	fputs("_:p2 <http://www.w3.org/1999/02/22-rdf-syntax-ns#first> ",file);
	sprintf(idtitle,"%s%lu%s","<https://ssejtsserver:9443/dm/models/",p2,"> ");
	fputs(idtitle,file);
	fputs(" .\n",file);
	
	fputs("_:p2 <http://www.w3.org/1999/02/22-rdf-syntax-ns#rest> _:p3 .\n",file);
	
	fputs("_:p3 <http://www.w3.org/1999/02/22-rdf-syntax-ns#first> ",file);
	sprintf(idtitle,"%s%lu%s","<https://ssejtsserver:9443/dm/models/",p3,"> ");
	fputs(idtitle,file);
	fputs(" .\n",file);
	
	fputs("_:p3 <http://www.w3.org/1999/02/22-rdf-syntax-ns#rest> _:<http://www.w3.org/1999/02/22-rdf-syntax-ns#nil> .\n",file);
	
	fputs(idtitle,file);
	fputs("<http://jazz.net/ns/dm/folder#folder> ",file);
	sprintf(idtitle,"%s%lu%s","<https://ssejtsserver:9443/dm/models/",folderid,"> ");
	fputs(idtitle,file);
}
void writepoint(FILE* file,char * name,unsigned long id, unsigned long folderid, float ** point){
	char idtitle[50];
	sprintf(idtitle,"%s%lu%s","<https://ssejtsserver:9443/dm/models/",id,"> ");
	writeheading(file,id,name,"<https://ssejtsserver:9443/dm/models/stlinterpreter#Point>");
	
	printXYZ(file,point,id);
	
	fputs(idtitle,file);
	fputs("<http://jazz.net/ns/dm/folder#folder> ",file);
	sprintf(idtitle,"%s%lu%s","<https://ssejtsserver:9443/dm/models/",folderid,"> ");
	fputs(idtitle,file);
} 

void printXYZ(FILE * file, float ** point, unsigned long id){
	char idtitle[50],temp[10];
	sprintf(idtitle,"%s%lu%s","<https://ssejtsserver:9443/dm/models/",id,"> ");

	fputs(idtitle,file);
	fputs("<https://ssejtsserver:9443/dm/models/stlinterpreter#x_val> \"",file);
	sprintf(temp,"%lu",(*point)[0]);
	fputs("\"^^<http://www.w3.org/2001/XMLSchema#double> .\n",file);
	
	fputs(idtitle,file);
	fputs("<https://ssejtsserver:9443/dm/models/stlinterpreter#y_val> \"",file);
	sprintf(temp,"%lu",(*point)[1]);
	fputs("\"^^<http://www.w3.org/2001/XMLSchema#double> .\n",file);
	
	fputs(idtitle,file);
	fputs("<https://ssejtsserver:9443/dm/models/stlinterpreter#z_val> \"",file);
	sprintf(temp,"%lu",(*point)[2]);
	fputs("\"^^<http://www.w3.org/2001/XMLSchema#double> .\n",file);
}
/*arguments:
	pointer to unsigned long representing number of triangle
	3 pointers to 2d arrays of floats representing coordinates of the 3 vertexes
	pointer of a 2d array of floats where all the points that are in the file will be written to
  return:number of points in the file
*/
unsigned long allPoints(unsigned long * numtriangles, float*** v1, float*** v2, float*** v3, float*** writeto){
	unsigned long numpoints = 0,i,maxpoints = *numtriangles + 2;
	float percent = 0.0;
	*writeto = (float **) malloc(sizeof(float*) * (unsigned int)(maxpoints));
	for(i = 0; i < maxpoints; ++i){
		(*writeto)[i] = (float*)malloc(sizeof(float)*3);
	}
	//goes through every triangle and writes to writeto any unique points
	for(i = 0; i < *numtriangles; ++i){
		if(!inSet(writeto,&numpoints,&((*v1)[i]))){
			(*writeto)[numpoints][0] = (*v1)[i][0];
			(*writeto)[numpoints][1] = (*v1)[i][1];
			(*writeto)[numpoints][2] = (*v1)[i][2];
			numpoints++;
		}
		if(!comparePoints(&((*v1)[i]),&((*v2)[i]))){
			if(!inSet(writeto,&numpoints,&((*v2)[i]))){
				(*writeto)[numpoints][0] = (*v2)[i][0];
				(*writeto)[numpoints][1] = (*v2)[i][1];
				(*writeto)[numpoints][2] = (*v2)[i][2];
				numpoints++;
			}
		}
		if(!comparePoints(&((*v1)[i]),&((*v3)[i])) && !comparePoints(&((*v2)[i]),&((*v3)[i]))){
			if(!inSet(writeto,&numpoints,&((*v3)[i]))){
				(*writeto)[numpoints][0] = (*v3)[i][0];
				(*writeto)[numpoints][1] = (*v3)[i][1];
				(*writeto)[numpoints][2] = (*v3)[i][2];
				numpoints++;
			}
		}
		//prints number of points read by intervals of 5 percent and will throw error if number of points is greater than the maximum number of points given the number of triangles
		if(i*100/(*numtriangles) > percent + 5){
			percent = ((i*100/(*numtriangles))/5)*5;
			printf("%.0f%% of points read in\n",percent);
		}
		if(numpoints > maxpoints){
			printf("ERROR: your stl file contains triangles that are not connected\n");
			break;
		}
	}
	printf("finished reading in all the points\n");
	return numpoints;
}
/*arguments:
	2 pointers to arays of floats representing the coordinates of the two points to be compared to
  return: bool represent whether the two coordinates were the same
*/
bool comparePoints(float ** p1, float **p2){
	return (*p1)[0] == (*p2)[0] && (*p1)[1] == (*p2)[1] && (*p1)[2] == (*p2)[2];
}
/*arguments:
	pointer to a 2D array of floats representing the collection of coordinates.
	pointer to an unsigned long representing size of the set
	pointer to a array of floating representing the coordinate in question
  return:bool representing whether the point was in the collection of points
*/
bool inSet(float *** set, unsigned long * size, float ** p){
	unsigned long i;
	for(i = 0; i < *size; ++i){
		if(comparePoints(&((*set)[i]),p)) return true;
	}
	return false;
}
/*arguments:
	file pointer of file to be read
	pointer to array of BYTES where the name shall be stored
	pointer to an unsigned long where the number of triangle in the file will be stored
	4 pointer to 2d arrays of floats where coordinates will be stored
	bool designating whether to be in debug modeor not
  return:bool whether the operation suceeded or not
 */
bool read(FILE * file, BYTE ** name, unsigned long * numtriangles, float*** normal, float*** v1, float*** v2, float*** v3,bool debug){
	unsigned long i;
	int j,k;
	BYTE * tempnumtriangle = (BYTE *) malloc(sizeof(BYTE) * 4);
	BYTE * temptrianglebuffer = (BYTE *) malloc(sizeof(BYTE) * 48);
	BYTE * throwaway = (BYTE *) malloc(sizeof(BYTE) * 2);
	//allocating memory for the name of the file and adding a nill terminator in the end
	*name = (BYTE*) malloc(sizeof(BYTE) * 81);
	if(!getName(file,name)) {
		if(debug){
			printf("coulden't get name of file\n");
		}
		return false;
	}
	(*name)[80] = '\0';
	if(debug){
		printf("name: %s \n",*name);
	}
	//getting the number of triangles in the file as a array of bytes and converting it to and unsigned long
	if(!getNumTriangle(file,&tempnumtriangle)) {
		if(debug){
			printf("coulden't get num of triangles\n");
		} 
		return false;
	}
	for(i = 0, *numtriangles = 0; i < 4; ++i){
		*numtriangles += (unsigned long) tempnumtriangle[i] << i*8;
	}
	//if in debug mode prints number of triangles and prints a warning if number of triangle exceedes the maximum unsigned ints
	if(debug){
		printf("num of triangles: %lu \n",*numtriangles);
	}
	if(*numtriangles >UINT_MAX){
		printf("note:number of triangles is greater than value of unsigned int. Program might not function correctly\n");
	}
	//allocating memory for the array of coordinates given the number of triangles
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
	//for the number of triangles read in coordinates of each triangle in bytes, converts to a float using a union and stores into the appropriate field.
	for(i = 0; i < *numtriangles; ++i){
		if(!getNextTriangle(file,&temptrianglebuffer,&throwaway)){
			if(debug){
				printf("coulden't get triangle %d\n",i);
			} 
			return false;
		}
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
	//freeing the temporary arrays used to read in the bytes from the file
	free(tempnumtriangle);
	free(temptrianglebuffer);
	free(throwaway);
	return true;
}
/*arguments:
	file pointer representing file to be read from
	pointer to array of BTYES to write name of file
  return:bool representing whether read was sucessful
*/
bool getName(FILE * file, BYTE ** buffer){
	size_t numread = fread((void*)(*buffer),1,80,file);
	return (numread == 80)?true:false;
}
/*arguments:
	file pointer representing file to be read from
	pointer to array of BYTE to store BYTE representation of number of triangles
  return:bool representing whether read was sucessful
*/
bool getNumTriangle(FILE * file, BYTE ** buffer){
	size_t numread = fread((void*)(*buffer),1,4,file);
	return (numread == 4)?true:false;
}
/*arguments:
	file pointer representing file to be read from
	pointer to array of BYTE to store coordinates of the triangle
	pointer to array of BYTE to store the contents of the throwaway
  return:bool representing whether read was sucessful
*/
bool getNextTriangle(FILE * file, BYTE ** buffer, BYTE ** throwaway){
	size_t numread = fread((void*)(*buffer),1,48,file);
	if(numread != 48) return false;
	numread = fread((void*)(*throwaway),1,2,file);
	return (numread == 2)?true:false;
}
/*arguments:
	pointer to 2d array of floate representing the array of coordinates to print out
	unsigned long representing number of elements in the array
	int representing number of floats in each coordinate
  returns:NULL
*/
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