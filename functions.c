/**
 * @file functions.c
 * @author Daniel Vilaça, António Rocha, Pedro Carneiro, Ana Silva
 * @brief Project backend
 * @date 2023-05-12
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "functions.h"


/**
 * @brief Convert a Unix timestamp to a date
 *
 * @param timestamp Unix timestamp
 * @return string with the date according to the timestamp
 */
char* ConvertUnixTimeStampToDate(time_t timestamp)
{
    long unixMinutes = timestamp/60;
    
    long unixSecondsOnMinute = timestamp % 60;
    if(unixMinutes <= 0 && unixSecondsOnMinute < 0)
    {
        unixSecondsOnMinute += 60;
        unixMinutes -= 1; 
    }

    long unixHours = unixMinutes/60;
    long unixMinutesOnHour = unixMinutes % 60;
    if(unixHours <=  0 && unixMinutesOnHour < 0)
    {
        unixMinutesOnHour += 60;
        unixHours -= 1;
    }

    long unixDays = unixHours/24;
    long unixHoursAtDay = unixHours % 24;
    if(unixDays <= 0 && unixHoursAtDay)
    {
        unixHoursAtDay += 24;
        unixDays -= 1;
    }

    long cycleOfFourHundredYears = unixDays/146097;
    long daysInFourHundredYears = unixDays % 146097;
    if(cycleOfFourHundredYears <= 0 && daysInFourHundredYears < 0)
    {
        daysInFourHundredYears += 146097;
        cycleOfFourHundredYears -= 1;
    }

    if(daysInFourHundredYears >= (32* 1461 + 789))
    {
        daysInFourHundredYears++;
        if(daysInFourHundredYears >= (57 * 1461 + 789))
        {
            daysInFourHundredYears++;
            if(daysInFourHundredYears >= (82 * 1461 +789))
            {
                daysInFourHundredYears++;
            }
        }
    }

    long fourYearCycles = daysInFourHundredYears/1461;
    long fourYearDays = daysInFourHundredYears % 1461;
    if(fourYearDays >= 59)
    {
        fourYearDays++;
        if(fourYearDays >= 425)
        {
            fourYearDays++;
            if(fourYearDays >= 1157)
            {
                fourYearDays++;
            }
        }

    }
    long yearInFourYears= fourYearDays/366;
    long daysAtYear = fourYearDays % 366;

    long finalYear = yearInFourYears + 4 * fourYearCycles + 400 * cycleOfFourHundredYears + 1970;

    long monthDays[] = {31,29,31,30,31,30,31,31,30,31,30,31};
    int month = 1;
    long days = daysAtYear;
    for(month = 1; month <= 12; month++)
    {
        if((days - monthDays[month-1]) > 0)
        {
            days -= monthDays[month-1];
        }
        else
        {
            break;
        }
    }
    
    days += 1;

     char* date = malloc(sizeof(char) * 100);
     sprintf(date,"%ld/%d/%ld", days,month,finalYear);

     return date;
}

/**
 * @brief Get the file stats of a file
 * 
 * @param fileDescriptor 
 * @param stats 
 * @return the stats of a file
 */
int GetFileStats(int fileDescriptor, struct stat * stats)
{
    if(fstat(fileDescriptor, stats) == -1)
        return -1;
    return 1;
}

/**
 * @brief Get the file size
 * 
 * @param fileDescriptor 
 * @return size if everything goes right else -1
 */
int _GetSize(int fileDescriptor)
{
    struct stat stats;
    if(GetFileStats(fileDescriptor, &stats) == -1) return -1;
    
    return stats.st_size;
}

/**
 * @brief Get the file information
 * 
 * @param fileDescriptor 
 * @return a pointer to file info
 */
FileInfo* GetFileInfo(int fileDescriptor)
{
    if(fileDescriptor == -1) return NULL;
    struct FileInfo* fileInfo = malloc(sizeof(FileInfo));
    if(fileInfo == NULL) return NULL;

    if(GetFileStats(fileDescriptor,&(fileInfo->stats)) == -1) return NULL;

    struct  passwd* user = getpwuid(fileInfo->stats.st_uid);
    if(user == NULL)
    {
        perror("Could not get user info!!\n");
        exit(1);
    }

    fileInfo->username = strdup(user->pw_name);
    if(fileInfo->username == NULL)
    {
        perror("Could not get the username!!\n");
        exit(1);
    }

    fileInfo->accessDate = ConvertUnixTimeStampToDate(fileInfo->stats.st_atim.tv_sec);

    fileInfo->modDate = ConvertUnixTimeStampToDate(fileInfo->stats.st_mtim.tv_sec);
    fileInfo->lastStatusChangeDate = ConvertUnixTimeStampToDate(fileInfo->stats.st_ctim.tv_sec);
    
    return fileInfo;

}

/**
 * @brief Destroy the file info object
 * 
 * @param fileInfo 
 * @return NULL
 */
FileInfo* DestroyFileInfo(FileInfo* fileInfo)
{
    if(fileInfo != NULL)
    {
        free(fileInfo->username);
        free(fileInfo);
    }
    return NULL;
}

/**
 * @brief Create a new file if exists it overrides the existing file
 * 
 * @param filename 
 * @return the new filedescriptor or error code
 */
int _CreateNewFile(const char* filename)
{
    int fileDescriptor = open(filename, O_CREAT|O_TRUNC|O_WRONLY,S_IWUSR|S_IRUSR);
    if(fileDescriptor == -1) return -1;
    return fileDescriptor;
}
/**
 * @brief Read a file to a buffer
 * 
 * @param fileSrc 
 * @param buffer 
 * @return number of bytes read or -1
 */
int _ReadFile(int fileSrc, char** buffer)
{
    if(fileSrc == -1) return -1;
    int fileSize = _GetSize(fileSrc);
    if(fileSize == -1) return -1;
    if(*buffer != NULL)
    {
        free(*buffer);
        *buffer = NULL;
        *buffer = malloc(sizeof(char) * fileSize);
        if(buffer == NULL) return -1;
    }
    else
    {
        *buffer = malloc(sizeof(char) * fileSize);
        if(buffer == NULL) return -1;
    }

    int bytesRead = read(fileSrc, *buffer, fileSize);

    if(bytesRead == -1) return -1;

    return bytesRead;

}

/**
 * @brief Write the content from a file to a file. Can be the stdout or another file
 * 
 * @param fileSrc 
 * @param fileDst 
 * @return number of bytes written
 */
int _WriteToFile(int fileSrc, int fileDst)
{

    if(fileSrc == -1 || fileDst == -1) return -1;

    
    char* buffer = NULL;
    int bytesRead = _ReadFile(fileSrc, &buffer);
    
    int bytesWritten = write(fileDst, buffer, bytesRead);
    if(bytesWritten == -1) return -1;

    free(buffer);
    return bytesWritten;

}

/**
 * @brief Show the content of a file
 * 
 * @param fileDescriptor 
 * @return bytes written to stdout
 */
int ShowFile(int fileDescriptor)
{

    if(fileDescriptor == -1) return -1;
    

    return _WriteToFile(fileDescriptor, STDOUT_FILENO);
}

/**
 * @brief concat the ".copy" string to a filename
 * 
 * @param filename 
 * @return concated string or NULL if something is wrong 
 */

char* _CreateFileNameCopy(const char* filename)
{
    char* new_filename = malloc(sizeof(filename) + 6);
    if(new_filename == NULL) return NULL;
    if(strcmp(strcpy(new_filename, filename),new_filename) != 0) return NULL;
    if(filename[sizeof(filename) -1] != '\0')
    strcat(new_filename, ".copy");

    return new_filename;
}

/**
 * @brief Creates a copy of a file to another. If dstFilename is NULL then it calls _CreateFileNameCopy function(see description) else it creates a file with
 * the user defined parameter
 * 
 * @param fileDescriptor 
 * @param filename 
 * @param dstFilename 
 * @return int 
 */
int CreateFileCopy(int fileDescriptor, const char* filename, char* dstFilename)
{
    char* new_filename;
    if(fileDescriptor == -1) return -1;
    if(dstFilename == NULL)
    {
       new_filename = _CreateFileNameCopy(filename);
    }
    else
    {
        new_filename = dstFilename;
    }
     
    int newFileDescriptor = _CreateNewFile(new_filename);

    
    if(newFileDescriptor == -1) return -1;

    return _WriteToFile(fileDescriptor, newFileDescriptor);
}

/**
 * @brief Append the content from a file to another
 * 
 * @param srcFile 
 * @param dstFile 
 * @return bytes written or -1 if something is wrong
 */
int AppendContent(int srcFile, int dstFile)
{
    if(srcFile == -1 || dstFile == -1) return -1;

    return _WriteToFile(srcFile, dstFile);

}

/**
 * @brief Count the lines from a file
 * 
 * @param srcFile 
 * @return number of lines or -1 if something is wrong
 */
int CountLines(int srcFile)
{
    if(srcFile == .1) return -1;

    char* buffer = NULL;
    int bytesRead = _ReadFile(srcFile,&buffer);
    if(bytesRead == -1) return -1;

    int lines = 0;
    char* token = strtok(buffer, "\n");
    while(token)
    {
        lines++;
        token = strtok(NULL, "\n");
    }
    return lines;
}

/**
 * @brief Deletes a file
 * 
 * @param filename 
 * @return success status
 */
int DeleteFile(const char* filename)
{
    if(filename == NULL) return -1;

    int status = unlink(filename);

    return status;
}

/**
 * @brief Lists a directory with the type of file and filename
 * 
 * @param dir 
 * @return success status
 */
int ListDirectory(const char* dir)
{
    DIR* directory = opendir(dir);
    if(directory == NULL) return -1;
    struct dirent* current = NULL;
    while((current = readdir(directory)) != NULL)
    {
        printf("Type of folder: %s, File Name: %s\n",current->d_type == DT_DIR? "Folder": "File", current->d_name);
    }

    return 1;
}