#ifndef FILE_H_
#define FILE_H_

#include "directory.h"

void create_file(const char *name);
void write_to_file(const char *name, const char *text);
void read_from_file(const char *name);
void delete_file_node(file_node *file_to_delete);

#endif
