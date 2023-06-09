/**
 * @file list.c
 * @author Daniel Vilaça, António Rocha, Pedro Carneiro, Ana Silva 
 * @brief list command entry point
 * @date 2023-05-12
 */


#include <stdlib.h>
#include "functions.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    char* filename;
    if(argc == 1)
    {
        filename = ".";
    }
    else if(argc == 2)
    {
        filename = argv[1];
    }
    else
    {
        fprintf(stderr, "Too many arguments!!");
        exit(EXIT_FAILURE);
    }


    
    if(ListDirectory(filename) == -1)
    {
        perror("Error listing directory");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}