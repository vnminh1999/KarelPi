#include "karelpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    karel_setup();
    /*
    move();
    turn_left();
    turn_right();
    wall_in_front();
    wall_to_left();
    wall_to_right();*/

    if(facing_north())
        printf("North\n");
    else if(facing_east())
        printf("East\n");
    else if(facing_south())
        printf("South\n");
    else
        printf("West\n");

    turn_off();
}