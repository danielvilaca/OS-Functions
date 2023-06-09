/**
 * @file functions.h
 * @author Daniel Vilaça, António Rocha, Pedro Carneiro, Ana Silva
 * @brief file manipulation header file
 * @date 2023-05-12
 */

#ifndef FILES_H
#define FILES_H

#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>

#include <dirent.h>

/**
 * @brief struct to store the file original information and processed information
 * 
 */
typedef struct FileInfo{
    char* username;
    struct stat stats;
    char* modDate;
    char* accessDate;
    char* lastStatusChangeDate;
}FileInfo;


int ShowFile(int file);
int CreateFileCopy(int file, const char* filename, char* dstFilename);
int AppendContent(int srcFile, int dstFile);
int CountLines(int srcFile);
int DeleteFile(const char* filename);
FileInfo* GetFileInfo(int fileDescriptor);
FileInfo* DestroyFileInfo(FileInfo* fileInfo);
int ListDirectory(const char* dir);
#endif