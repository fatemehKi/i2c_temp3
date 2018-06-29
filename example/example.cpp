/*
 * example.cpp
 *
 *  Created on: Jun 26, 2018
 *      Author: fatemeh
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>

#include "AdafruitMCP9808.h"

/*
int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    /* set the terminal to raw mode
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    /* read a character from the stdin stream without blocking
    /*   returns EOF (-1) if no character is available
    character = fgetc(stdin);

    /* restore the original terminal attributes
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}
*/
int main() {
    AdafruitMCP9808 *adafruitMCP9808 = new AdafruitMCP9808() ;
    int err = adafruitMCP9808->openAdafruitMCP9808();
    if (err < 0){
        printf("Error: %d", adafruitMCP9808->error);
    }

       // int hardwareVersion = AdafruitMCP9808->getHardwareVersion() ;
        //int softwareVersion = AdafruitMCP9808->getSoftwareVersion() ;
       // printf("Hardware Version: %d\n",hardwareVersion) ;
        //printf("Software Version: %d\n",softwareVersion) ;

        // 27 is the ESC key

        while(adafruitMCP9808->error >= 0 ){
            int distance = adafruitMCP9808->getTemp();
            if (distance < 0) {
                int llError ;
                llError = adafruitMCP9808->getError() ;
                printf("Adafruit MCP9808 error: %d\n",llError) ;
        }
    }
    adafruitMCP9808->closeAdafruitMCP9808();
}
