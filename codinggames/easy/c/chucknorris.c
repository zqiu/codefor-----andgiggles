// Read inputs from stdin. Write outputs to stdout.

#include <stdlib.h>
#include <stdio.h>

int main()
{
    int n,i,j;
    char buffer[100],cur = '\x02';
    //scanf("%d", &n);
    gets(buffer);
    //fprintf(stderr,"%s\n",buffer);

    n = strlen(buffer);
    for (i = 0; i < n; i++) {
        for(j = 6; j >= 0; --j){
            if(cur != (buffer[i] >> j)%2){
                //fprintf(stderr,"%d\n",(buffer[i]>>j)%2);
                //fprintf(stderr,"cur:%d\n",cur);
                if(cur != '\x02'){
                   printf(" ");
                }
                cur = (buffer[i] >> j)%2;
                if(cur){
                    printf("0 0");
                }else{
                    printf("00 0");
                }
            }else{
                printf("0");
            }
        }
    }
    printf("\n");
    return 0;
}