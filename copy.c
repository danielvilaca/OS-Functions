/**
 * @file copy.c
 * @author Daniel Vilaça, António Rocha, Pedro Carneiro, Ana Silva 
 * @brief copy command entry point
 * @date 2023-05-12
 */

#include <stdlib.h>
#include "functions.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    char* dstFilename;
    if(argc ==2)
    {
        dstFilename = NULL;
    }
    else if(argc == 3)
    {
        dstFilename = argv[2];
    }
    else
    {
        fprintf(stderr,"Too few or many arguments!!!");
    }

    int fileDescriptor = open(argv[1], O_RDONLY);
    if(fileDescriptor == -1)
    {
        perror("Error reading file: ");
        exit(EXIT_FAILURE);
    }

    int status = CreateFileCopy(fileDescriptor, argv[1], dstFilename);
    if(status == -1)
    {
        perror("Error showing file: ");
        exit(EXIT_FAILURE);
    }

    printf("File copied sucessfully!!\n");
    return EXIT_SUCCESS;
}