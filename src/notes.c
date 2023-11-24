#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>

#include "files_handler.c"

typedef struct
{
    int id;
    char filename[30];
    char created_at[11];
} Note;

char *get_current_formatted_time(time_t date)
{
    char *formatted_date;
    struct tm *timeinfo = localtime(&date);
    strftime(formatted_date, sizeof(char[11]), "%d/%m/%Y", timeinfo);
    return formatted_date;
}

GList *push(GList *notes, int id, char *filename, char *_formatted_date)
{
    char formatted_date[11];
    if (!_formatted_date)
    {
        time_t date = time(NULL);
        strcpy(formatted_date, get_current_formatted_time(date));
    }
    else
    {
        strcpy(formatted_date, _formatted_date);
    }
    Note *_note = g_new(Note, 1);
    _note->id = id;
    strcpy(_note->filename, filename);
    strcpy(_note->created_at, _formatted_date ? _formatted_date : formatted_date);
    return g_list_append(notes, _note);
}

GList *init_notes()
{
    GList *notes = NULL;
    return notes;
}

GList *get_notes()
{
    FILE *fp;
    fp = open_db_file();
    GList *notes = init_notes();

    char response[60];
    int rows_length = get_row(fp, response);
    int id = 1;
    while (rows_length > 0)
    {
        char *str = get_str(response);
        char *items[2];
        int index = 0;
        while (str != NULL)
        {
            items[index] = str;
            index++;
            str = get_str(NULL);
        }

        notes = push(notes, id, items[0], items[1]);
        rows_length = get_row(fp, response);
        id++;
    }
    fclose(fp);

    return notes;
}

long get_file_size(FILE *fp)
{

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

char *get_note_content(char *filename)
{
    FILE *fp;
    fp = open_note_file(filename, "r");
    if (!fp)
    {
        printf("Unable to start fp (FILE*) object.");
        exit(0);
    }
    long size = get_file_size(fp);
    char *response = malloc(size);
    fread(response, 1, size, fp);
    fclose(fp);
    return response;
}

void set_note_content(char *filename, char *content)
{
    FILE *fp;
    fp = open_note_file(filename, "w+");
    if (!fp)
    {
        printf("Unable to start fp (FILE*) object in write mode.");
        exit(0);
    }
    fputs(content, fp);
    fclose(fp);
}

// int main()
// {
//     // char *content = get_note_content("file1.txt");
//     get_note_content("file_1.txt");
//     // printf("%s\n", get_note_content("file1.txt"));
//     return 0;
// }

// int main()
// {
//     FILE *fp;
//     fp = open_db_file();
//     GList *notes = init_notes();

//     char response[60];
//     int rows_length = get_row(fp, response);
//     int id = 1;
//     while (rows_length > 0)
//     {
//         char *str = get_str(response);
//         char *items[2];
//         int index = 0;
//         while (str != NULL)
//         {
//             items[index] = str;
//             index++;
//             str = get_str(NULL);
//         }

//         notes = push(notes, id, items[0], items[1]);
//         rows_length = get_row(fp, response);
//         id++;
//     }

//     printf("%s\n", ((Note *)(notes->data))->filename);
//     printf("%s\n", ((Note *)(notes->data))->created_at);

//     fclose(fp);

//     return 0;
// }

// int main()
// {

//     GList *notes = NULL;

//     notes = push(notes, 1, "file1.txt");

//     printf("%d\n", ((Note *)(notes->data))->id);
//     printf("%s\n", ((Note *)(notes->data))->filename);
//     printf("%s\n", ((Note *)(notes->data))->created_at);
//     printf("%ld\n", ((Note *)(notes->data))->created_at_timestamp);

//     g_list_free(notes);

//     return 0;
// }