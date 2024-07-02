#ifndef CSV_PARSER_H
#define CSV_PARSER_H

void remove_whitespaces(char* input);
void parse_csv(const char* input_filename, struct Request* requests, int numRequests);

#endif // CSV_PARSER_H
