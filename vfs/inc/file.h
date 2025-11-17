#ifndef FILE_H_
#define FILE_H_


#include "directory.h"


void create_file(const char *name);
void write_file(const char *name, const char *text);
void read_file(const char *name);
void delete_file_node(file_node *temp);

#endif 