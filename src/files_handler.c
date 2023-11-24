#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "files_handler.h"

char *get_db_file_path()
{
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    return strcat(cwd, "/db/db.txt");
}

char *get_files_path()
{
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    return strcat(cwd, "/db/files/");
}

FILE *open_db_file()
{
    FILE *fp;
    char db_file_path[256];
    strcpy(db_file_path, get_db_file_path());
    return fopen(db_file_path, "r");
}

FILE *open_note_file(char *filename, char mode[1])
{
    FILE *fp;
    char files_path[256];
    strcpy(files_path, get_files_path());
    char complete_path[256];
    strcpy(complete_path, strcat(files_path, filename));
    return fopen(complete_path, mode);
}

int get_row(FILE *fp, char response[60])
{
    return fscanf(fp, "%s\n", response);
}

char *get_str(char row[60])
{
    return strtok(row, ";");
}
