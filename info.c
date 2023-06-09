/**
 * @file info.c
 * @author Daniel Vilaça, António Rocha, Pedro Carneiro, Ana Silva 
 * @brief info command entry point
 * @date 2023-05-12
 */

#include <stdlib.h>
#include "functions.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


/**
 * @brief Print the file information
 * 
 * @param fileInfo 
 * @return 1
 */
int PrintFileStats(FileInfo* fileInfo)
{
    if(S_ISREG(fileInfo->stats.st_mode))
    {
        printf("File Type: Ficheiro normal!!\n");
    }
    else if(S_ISCHR(fileInfo->stats.st_mode))
    {
        printf("File Type: Dispositivo orientado ao caracter\n");
    }
    else if(S_ISBLK(fileInfo->stats.st_mode))
    {
        printf("File Type: Dispositivo orientado ao bloco\n");
    }
    else if(S_ISFIFO(fileInfo->stats.st_mode))
    {
        printf("File Type: Pipe\n");
    }
    else if(S_ISDIR(fileInfo->stats.st_mode))
    {
        printf("File Type: Pasta/Diretoria\n");
    }
    else if(S_ISLNK(fileInfo->stats.st_mode))
    {
        printf("File Type: Atalho/Link\n");
    }
    else if(S_ISSOCK(fileInfo->stats.st_mode))
    {
        printf("File Type: Socket\n");
    }
    else
    {
        printf("File Type: Desconhecido!!\n");
    }
    
    printf("I-node: %ld\n", fileInfo->stats.st_ino);
    printf("File owner: %s\n", fileInfo->username);
    printf("Access Date: %s \n",fileInfo->accessDate);
    printf("Modification Date: %s\n", fileInfo->modDate);
    printf("Last status change Date: %s\n", fileInfo->lastStatusChangeDate);

    return 1;
}

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
    FileInfo* info = GetFileInfo(fileSrc);
    if(info == NULL)
    {
        perror("Not able to get file info: ");
        exit(EXIT_FAILURE);
    }

    PrintFileStats(info);

    return EXIT_SUCCESS;
}