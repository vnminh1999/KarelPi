#include "karelpi.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    karel_setup();
    move();
    turn_left();
    turn_right();
    facing_east();
    turn_off();
}