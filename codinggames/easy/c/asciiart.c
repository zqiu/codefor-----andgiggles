// Read inputs from stdin. Write outputs to stdout.

#include <stdlib.h>
#include <stdio.h>

int main()
{
    int l,h,i,j,pos;
    char letter[201], ** buffer, *temp;
    scanf("%d%d", &l,&h);
    fgetc(stdin);gets(letter);
    fprintf(stderr,"%d,%d,%s\n",l,h,letter);
    buffer = (char**)malloc(h*sizeof(char*));
    temp = (char *)malloc(l+1);
    for(i = 0; i < h; ++i){
        buffer[i] = (char*)malloc(l*27*sizeof(char) + 1);
    }
    //fgetc(stdin);//ignore the blank line after string
    for(i = 0; i < h; ++i){
        //scanf("%s",buffer[i]);
        gets(buffer[i]);
        //fprintf(stderr,"scanned in %s\n",buffer[i]);
    }
    
    for(i = 0; i < h; ++i){
        for(j = 0; j < strlen(letter); ++j){
            if(letter[j] >= 'a' &&  letter[j] <= 'z'){
                pos = letter[j] - 'a';
            }else if(letter[j] >= 'A' && letter[j] <= 'Z'){
                pos = letter[j] - 'A';
            }else{
                pos = 26;
            }
            temp = strncpy(temp,&buffer[i][pos * l],l);
            temp[l]='\0';
            printf("%s",temp);
        }
        printf("\n");
    }
    
    for(i = 0; i < h; ++i){
        free(buffer[i]);
    }
    free(buffer);
    free(temp);
    
    return 0;
}