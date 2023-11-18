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

FILE *open_db_file()
{
    FILE *fp;
    char db_file_path[256];
    strcpy(db_file_path, get_db_file_path());
    return fopen(db_file_path, "r");
}

int get_row(FILE *fp, char response[60])
{
    return fscanf(fp, "%s\n", response);
}

char *get_str(char row[60])
{
    return strtok(row, ";");
}

// int main()
// {
//     FILE *fp;
//     fp = open_db_file();

//     char response[60];
//     int rows_length = get_row(fp, response);
//     while (rows_length > 0)
//     {
//         char *str = get_str(response);
//         while (str != NULL)
//         {
//             printf("1> %s\n", str);
//             str = get_str(NULL);
//         }
//         printf("-\n");
//         rows_length = get_row(fp, response);
//     }

//     fclose(fp);

//     return 0;
// }