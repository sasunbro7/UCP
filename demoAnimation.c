#include<stdio.h>
#include<stdlib.h>
#include"newSleep.h"
#include"color.h"

int main(){

    /* This source code shows a short demonstration on how to create a simple animation to be displayed on a terminal interface. Animation is made from sequence of images (frames) that shows continuous movement. */

    /* In this example, the animation continues for a specific number of frames. However, you can also modify the code so that the animation stops after completing a specific task, such as "stop after 5 bounces" or "stop after reaching the corner of the map" */

    /* Please watch the other supplementary videos about *terminal screen clearing*, *making a program sleeps*, and *changing foreground and background colours* since we will use codes from those topics. */

    int option = -1;
    int maxFrames = 50; /* amount of animation frames */

    while(option != 1 && option != 2){
        system("clear");
        printf("(1) Linear movement\n(2) Bouncing ball\n");
        printf("please choose option 1 or 2 : ");
        scanf("%d", &option);
    }

    if(option == 1){ /* linear movement */
        int positionColour = 0;
        int change = 1; /* 1 or -1 depending on direction */
        int length = 8; /* length of the bar. Minimum is 2. */
        int index;
        int frame = 1;

        system("clear");

        while(frame <= maxFrames){
            system("tput cup 0 0");
            
            /* top border */
            for(index = 0 ; index < length + 2 ; index++){
                printf("#");
            }
            printf("\n");

            printf("#");
            for(index = 0 ; index < length ; index++){
                if(index == positionColour){
                    setBackground("red");
                }
                printf(" ");
                setBackground("reset");
            }
            printf("#\n");

            /* bottom border */
            for(index = 0 ; index < length + 2 ; index++){
                printf("#");
            }
            printf("\n");

            positionColour = positionColour + change;
            if     (positionColour == length-1){ change = -1; }
            else if(positionColour == 0){ change = 1; }
            
            frame++;
            newSleep(0.2); /* time gap between frame. Lower this to increase animation speed. */
        }

    }
    else if(option == 2){ /* bouncing ball */
        int rowSize = 6;
        int colSize = 10; /* size of area. Minimum is 2. */
        
        int rowBall = 3;
        int colBall = 6; /* initial position of the ball. has to be within playable area. */
        
        int rowChange = 1; /* 1 or -1 depending on direction */
        int colChange = 1;

        int rr, cc;
        int frame = 1;

        system("clear");

        while(frame <= maxFrames){
            system("tput cup 0 0");

            if(rowBall == rowSize - 1){ rowChange = -1; }
            if(rowBall == 0){ rowChange = 1; }
            if(colBall == colSize - 1){ colChange = -1; }
            if(colBall == 0){ colChange = 1; }

            /* top border */
            for(cc = 0 ; cc < colSize + 2 ; cc++){
                printf("#");
            }
            printf("\n");

            for(rr = 0 ; rr < rowSize ; rr++){
                printf("#");
                for(cc = 0 ; cc < colSize ; cc++){
                    if(rr == rowBall && cc == colBall){
                        setBackground("red"); /* red-coloured ball */
                    }
                    printf(" ");
                    setBackground("reset");
                }
                printf("#\n");
            }

            /* bottom border */
            for(cc = 0 ; cc < colSize + 2 ; cc++){
                printf("#");
            }
            printf("\n");

            rowBall = rowBall + rowChange;
            colBall = colBall + colChange;

            frame++;
            newSleep(0.2);
        }
    }

    return 0;
}