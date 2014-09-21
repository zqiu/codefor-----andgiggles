#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int LX; // the X position of the light of power
    int LY; // the Y position of the light of power
    int TX; // Thor's starting X position
    int TY; // Thor's starting Y position
    scanf("%d%d%d%d", &LX, &LY, &TX, &TY); fgetc(stdin);
    int NX = TX; //x position now
    int NY = TY; //y position now
    char north,east;
    // game loop
    while (1) {
        int E; // The level of Thor's remaining energy, representing the number of moves he can still make.
        scanf("%d", &E); fgetc(stdin);
        // Write an action using printf(). DON'T FORGET THE TRAILING \n
        // To debug: fprintf(stderr, "Debug messages...\n");
        if(TX < LX){
            east = 1;
        }else if(TX > LX){
            east = 2;
        }else{
            east = 0;
        }
        if(TY > LY){
            north = 1;
        }else if(TY < LY){
            north = 2;
        }else{
            north = 0;
        }
//        printf("SE\n"); // A single line providing the move to be made: N NE E SE S SW W or NW

        if(!east){
            printf((north==1)?"N\n":"S\n");
        }else if(!north){
            printf((east==1)?"E\n":"W\n");
        }else if(east == 1){
            printf((north == 1)?"NE\n":"SE\n");
        }else{
            printf((north == 1)?"NW\n":"SW\n");
        }
        if(east){TX += (east == 1)?1:-1;}
        if(north){TY += (north != 1)?1:-1;}
    }
}