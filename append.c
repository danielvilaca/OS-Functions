/**
 * @file append.c
 * @author Daniel Vilaça, António Rocha, Pedro Carneiro, Ana Silva
 * @brief append entry point
 * @date 2023-05-12
 */

#include <stdlib.h>
#include "functions.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr,"ERROR: Too few arguments!!");
        exit(EXIT_FAILURE);
    }

    int fileSrc = open(argv[1], O_RDONLY);
    int fileDst = open(argv[2], O_APPEND|O_WRONLY);


    if(fileSrc == -1)
    {
        perror("Error reading source file: ");
        exit(EXIT_FAILURE);
    }

    if(fileDst == -1)
    {
       perror("Error reading destination file: ");
       exit(EXIT_FAILURE); 
    }



    int status = AppendContent(fileSrc,fileDst);
    if(status == -1)
    {
        perror("Error append file: ");
        exit(EXIT_FAILURE);
    }

    printf("File append ended sucessfully!!\n");
    return EXIT_SUCCESS;
}