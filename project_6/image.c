#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "image.h"

//global variable representing open file
int image_fd;


//This opens the image file of the given name, creating it if it doesn't exist, 
//and truncating it to 0 size if truncate is true (see below).
    int image_open(char *filename, int truncate){

        if (truncate == 1){
            image_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0600);
        }
        else{
            image_fd = open(filename, O_RDWR | O_CREAT, 0600);
        }
        if (image_fd == -1) {
            perror("open");
        }
        return image_fd;


    } 

//This closes the image file.
    int image_close(void){

        return close(image_fd);
        
    } 
