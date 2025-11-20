#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

int calculate_string_length(const char *str);
void parse_input_string(const char *input_string, char args_array[3][1000]);
int is_valid_name(const char *name);

#endif
