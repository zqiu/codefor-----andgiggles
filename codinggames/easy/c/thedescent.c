#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    // game loop
    while (1) {
        int SX; int SY; int ans,max = 0;
        scanf("%d%d", &SX, &SY); fgetc(stdin);
        for (int i = 0; i < 8; i++) {
            int MH; // represents the height of one mountain, from 9 to 0. Mountain heights are provided from left to right.
            scanf("%d", &MH); fgetc(stdin);
            if(max < MH || max == 0){
                max = MH;
                ans = i;
                
                fprintf(stderr,"SX:%d,max:%d,ans:%d",SX,max,i);
            }
        }
        
        if(SX == ans){
            printf("FIRE\n");
        }else{

        // Write an action using printf(). DON'T FORGET THE TRAILING \n
        // To debug: fprintf(stderr, "Debug messages...\n");

            printf("HOLD\n"); // either:  FIRE (ship is firing its phase cannons) or HOLD (ship is not firing).
        }
    }
}