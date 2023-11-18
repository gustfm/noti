#ifndef FILES_HANDLER_H
#define FILES_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *get_db_file_path();
FILE *open_db_file();
int get_row(FILE *fp, char response[60]);
char *get_str(char row[60]);

#endif