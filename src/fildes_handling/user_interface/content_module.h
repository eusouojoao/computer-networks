#ifndef CONTENT_MODULES_H
#define CONTENT_MODULES_H

#include "../../common/struct.h"

names *new_names(char *name, names *next);
int insert_name(host *host, char *name);
int delete_name(host *host, char *delname);
void get_name(host *host, char *buffer);

#endif // !CONTENT_MODULES_H
