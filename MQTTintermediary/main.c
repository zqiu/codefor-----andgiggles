#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char ** argv){
}

unsigned int getresponse(char* msg){
	printf("message to send?\n");
	scanf("%s",msg);
	return strlen(msg);
}

void sendresponse(char* msg){
	printf("%s\n",msg);
}

char* getuftstring(char* src, unsigned int length){
	unsigned int i;
	char * temp = (char*)malloc(sizeof(char) * length);
	strncpy(temp,src,length);
	src = (char*)realloc(src,sizeof(char)*(length+2));
	src[0] = (char)(length>>8);
	src[1] = (char)(length);
	for(i = 0; i < length; ++i){
		src[i+2] = temp[i];
	}
	free(temp);
	return src;
}

void error(char* msg){
	printf("error:%s\n",msg);
	exit(EXIT_FAILURE);
}

void connect(){
	unsigned int len;
	char buffer[] = {0x10,0x00,0x06,'M','Q','I','s','d','p',0x03,0xCA,0x00,0x0A};
	sendresponse(buffer);
	buffer = (char*)realloc(buffer,sizeof(char) * 80));
	printf("client id?\n");
	scanf("%s",buffer);
	getuftstring(buffer,strlen(buffer));
	sendresponse(buffer);
	printf("username?\n");
	scanf("%s",buffer);
	getuftstring(buffer,strlen(buffer));
	sendresponse(buffer);
	printf("password?\n");
	scanf("%s",buffer);
	getuftstring(buffer,strlen(buffer));
	sendresponse(buffer);
	len = getresponse(buffer);
	if(len != 3) error("did not recieve a correct message from server");
	if(buffer[0] != 0x20) error("did not recieve a CONNACK from server");
	if(buffer[2] != 0x00){
		if(buffer[2] == 0x01)error("wrong protocol version");
		else if(buffer[2] == 0x02) error("identifier rejected");
		else if(buffer[2] == 0x03) error("server unavailable");
		else if(buffer[2] == 0x04) error("wrong user/pass");
		else if(buffer[2] == 0x05) error("not authorized");
		else error("unrecognized error");
	}
	printf("connected sucessfully with server\n");
	free(buffer);
}