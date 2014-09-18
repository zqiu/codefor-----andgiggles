#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

//#define DEBUGPERM
//#define DBGSBOX
//#define DBGFFUN
//#define VERBOSE

#define KEYPERM1 		"\x03\x05\x02\x07\x04\x0A\x01\x09\x08\x06"
#define KEYPERM2 		"\x06\x03\x07\x04\x08\x05\x0A\x09"
#define FFUNCTIONEXP 	"\x04\x01\x02\x03\x02\x03\x04\x01"
#define FFUNCTIONPERM	"\x02\x04\x03\x01"
#define SBOXK1			"\x73\x29\xD7\x8E"
#define SBOXK2			"\x2E\x41\x94\xF3"
#define IPERM			"\x02\x06\x03\x01\x04\x08\x05\x07"
#define IIPERM			"\x04\x01\x03\x05\x07\x02\x08\x06"

void permutation(char * source, char * ans, char * permutation, char befores, char afters);
void getkeys(char* source, char* round1, char* round2);
void shiftleft(char * toshift);
char submatrix(char * source,unsigned char * key);
char ffunction(char source, char * key);

int main(int argc, char ** argv){
	char key[2],plaintext[2],partialk1[2],partialk2[2];
	char ciphertext[2],scratch[2],temp,itr;
	if(argc != 5){
		printf("error: need arg to represent first and last 4 bits of plain text\n");
		printf("and to represent first and last 5 bits of key\n");
		exit(EXIT_FAILURE);
	}
	plaintext[0] = (char)atoi(argv[1]);
	plaintext[1] = (char)atoi(argv[2]);
	key[0] = (char)atoi(argv[3]);
	key[1] = (char)atoi(argv[4]);
	getkeys(key,partialk1,partialk2);
	permutation(plaintext,scratch,IPERM,8,8);
	
	temp = ffunction(scratch[1],partialk1);
	temp = temp ^ scratch[0];
	scratch[0] = scratch[1];
	scratch[1] = temp;
#ifdef VERBOSE
	printf("result after XOR:%d,%d\n",scratch[0],scratch[1]);
#endif
	
	temp = ffunction(scratch[1],partialk2);
	temp = scratch[0] = temp ^ scratch[0];
#ifdef VERBOSE
	printf("result after XOR:%d,%d\n",scratch[0],scratch[1]);
#endif
	
	permutation(scratch,ciphertext,IIPERM,8,8);
	printf("your cipher is ");
	for(itr = 0; itr < 8; ++itr){
		if(itr < 4){
			printf("%d",(ciphertext[0] >> (3-itr))%2);
		}else{
			printf("%d",(ciphertext[1] >> (7-itr))%2);
		}
	}
	printf("\n");
}

void getkeys(char* source, char* round1, char* round2){
	char i,temp[2],ansr1[2],ansr2[2];
	permutation(source,round1,KEYPERM1,10,10);
	shiftleft(round1);
	round2[0] = round1[0];
	round2[1] = round1[1];
	permutation(round1,ansr1,KEYPERM2,10,8);
	shiftleft(round2);
	//shiftleft(round2);		//were we supposed to shift left twice in the second round?
	permutation(round2,ansr2,KEYPERM2,10,8);
	for(i = 0; i < 2; ++i){
		round1[i] = ansr1[i];
		round2[i] = ansr2[i];	
	}
#ifdef VERBOSE
	printf("Round 1 keys generated:%d,%d\n",round1[0],round1[1]);
	printf("Round 2 keys generated:%d,%d\n",round2[0],round2[1]);
#endif	
}

void permutation(char * source, char * ans, char * permutation, char befores, char afters){
	char i,j,toadd,toshift;
#ifdef DEBUGPERM
	char temp;
	printf("testing what happens to the number after you shift it\n");
	for(i = 0; i < befores; ++i){
		if(i < befores/2){
			j = 0;
		}else{
			j = 1;
		}
		temp = source[j] >> i%(befores/2);
		printf("%d,%d\n",i,temp);
	}
	printf("\n");
#endif
	ans[0] = (char)0;
	ans[1] = (char)0;
	for(i = 0; i < afters; ++i){
		if(i < afters/2){
			j = 0;
		}else{
			j = 1;
		}
		ans[j] = ans[j]<< 1;
		//determines how much source needs to be shifted to get the right bit. 
		//for a 10 bit permutation 1,2,3,4,5,6,7,8,9,10 map to 4,3,2,1,0,4,3,2,1,0
		toshift = ((befores/2)-(permutation[i]%(befores/2)))%(befores/2);
		//if the permutation calls for 1-5 in a 10-bit perm, take from first half. Else from second
		toadd = source[(permutation[i] <= (befores/2))?0:1] >> toshift;
		ans[j] += (toadd%2);
#ifdef DEBUGPERM
		printf("%d:perm%d  :shift%d  :add%d  :ans[0]%d  :ans[1]%d\n",i,permutation[i],toshift,toadd,ans[0],ans[1]);
#endif
	}
#ifdef VERBOSE
	printf("permutation from %d to %d called, source:%d,%d ans:%d,%d\n",befores,afters,source[0],source[1],ans[0],ans[1]);
#endif
}

void shiftleft(char * toshift){
	char i, storage[2];
	for(i = 0; i < 2; ++i){
		storage[i] = toshift[i];
		if((toshift[i] >> 4)%2){
			toshift[i] = ((toshift[i] << 1) + 1)%32;
		}else{
			toshift[i] = (toshift[i] << 1)%32;
		}
	}
#ifdef VERBOSE
	printf("shifting %d,%d to the left one. Result %d,%d\n",storage[0],storage[1],toshift[0],toshift[1]);
	printf("MSB was:%d,%d\n",(storage[0] >> 4)%2,(storage[1] >> 4)%2);	
#endif
}

char submatrix(char * source,unsigned char * key){
	unsigned char ans,toshift;
	ans = key[source[0]];
	toshift = 6 - (2*source[1]);
#ifdef DBGSBOX
	printf("ans is %d shifted %d\n",ans,toshift);
#endif
	ans = (ans >> toshift)%4;
#ifdef VERBOSE
	printf("submatrix computed %d,source 0:%d,source 1:%d\n",ans,source[0],source[1]);
#endif
	return ans;
}

//get in source and key for ffunction and return ans
char ffunction(char source, char * key){
	char sourceperm[2],temp[2],sourcesplit[2],ans[2],toreturn;
	sourceperm[0] = (source >> 2)%4;
	sourceperm[1] = source%4;
	permutation(sourceperm,temp,FFUNCTIONEXP,4,8);
	temp[0] = temp[0] ^ key[0]; //XOR
	temp[1] = temp[1] ^ key[1];
#ifdef DBGFFUN
	printf("result after XOR:%d,%d\n",temp[0],temp[1]);
#endif
	//get second bit and shift it one and add third bit
	sourcesplit[0] = ((temp[0] >> 2)%2 << 1)+(temp[0]>>1)%2;	
	//get first bit and shift it one and add last bit
	sourcesplit[1] = ((temp[0] >> 3)%2 << 1)+temp[0]%2;
	temp[0] = submatrix(sourcesplit,SBOXK1);
	//get second bit and shift it one and add third bit
	sourcesplit[0] = ((temp[1] >> 2)%2 << 1)+(temp[1]>>1)%2;	
	//get first bit and shift it one and add last bit
	sourcesplit[1] = ((temp[1] >> 3)%2 << 1)+temp[1]%2;
	temp[1] = submatrix(sourcesplit,SBOXK2);
	permutation(temp,ans,FFUNCTIONPERM,4,4);
	toreturn = (ans[0] << 2) + (ans[1] % 4);
#ifdef VERBOSE
	printf("ffunction computed,ans:%d,source:%d\n",toreturn,source);
#endif
	return toreturn;
}