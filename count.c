/**
 * @file count.c
 * @author Daniel Vilaça, António Rocha, Pedro Carneiro, Ana Silva
 * @brief count command entry point
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

    int fileSrc = open(argv[1], O_RDONLY);
    


    if(fileSrc == -1)
    {
        perror("Error reading source file: ");
        exit(EXIT_FAILURE);
    }

    int status = CountLines(fileSrc);
    if(status == -1)
    {
        perror("Error counting lines of file: ");
        exit(EXIT_FAILURE);
    }

    printf("Number of lines: %d\n", status);
    return EXIT_SUCCESS;
}