#ifndef NOTES_H
#define NOTES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>

char *get_current_formatted_time(time_t date);

GList *push(GList *notes, int id, char *filename, char *_formatted_date);

GList *init_notes();

#endif