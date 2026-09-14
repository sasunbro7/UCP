#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"color.h"

int main()
{
    int arr[5][5] = { {1,2,3,4,5} , {6,7,8,9,10} , {11,12,13,14,15} , {16,17,18,19,20} , {21,22,23,24,25}};
    int i, j;

    setForeground("red");           /*   set foreground color before printing */
    printf("red fg\n");
    setForeground("reset");         /* reset foreground color after  printing */


    setBackground("magenta");       /*   set background color before printing */
    printf("magenta background");   
    setBackground("reset");         /* reset background color after  printing */
    printf("\n");

    setForeground("blue");
    setBackground("white");
    printf("blue fg, white bg");    /* changing both foreground and background color */
    setForeground("reset");
    setBackground("reset");
    printf("\n");

    for(i=0 ; i<5 ; i++)
    {
        for(j=0 ; j<5 ; j++)
        {
            printf("%02d ", arr[i][j]);     /* printing content of 2D array without color */
        }
        printf("\n");
    }
    printf("\n");


    for(i=0 ; i<5 ; i++)
    {
        for(j=0 ; j<5 ; j++)
        {
            if( (i+j) % 2 == 0)
            {
                setBackground("white");
            }
            else
            {
                setBackground("blue");
            }
            printf("%02d ", arr[i][j]);     /* printing content of 2D array with alternative color */

            setBackground("reset");
        }
        printf("\n");
    }
    printf("\n");



    return 0;
}