/**
 * @file mostra.c
 * @author Daniel Vilaça, António Rocha, Pedro Carneiro, Ana Silva 
 * @brief show command entry point
 * @date 2023-05-12
 */

#include <stdlib.h>
#include "functions.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr,"ERROR: Too few arguments!!");
        exit(EXIT_FAILURE);
    }

    int fileDescriptor = open(argv[1], O_RDONLY);
    if(fileDescriptor == -1)
    {
        perror("Error reading file: ");
        exit(EXIT_FAILURE);
    }

    int status = ShowFile(fileDescriptor);
    if(status == -1)
    {
        perror("Error showing file: ");
        exit(EXIT_FAILURE);
    }

    printf("\n");
    return EXIT_SUCCESS;
}