// Read inputs from stdin. Write outputs to stdout.

#include <stdlib.h>
#include <stdio.h>

int main()
{
    int n,tempnow,closest=0,abstemp;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &tempnow);
        abstemp = abs(tempnow);
        if(abs(closest) > abstemp || closest == 0 || 
            ((tempnow > 0) && (abs(closest) == abstemp))){
            closest = tempnow;
        }
    }
    printf("%d\n",closest);
    return 0;
}