// Lie Leon Alexius

#include "csv_parser.h"

/**
 * @brief removes whitespaces from a string
 * @note `"Le o n"` would be `"Leon"` and `" "` would be `""`
 * @author Lie Leon Alexius
 */
void remove_whitespaces(char* input) {
    int n = strlen(input);
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (input[i] != ' ') {
            input[j++] = input[i];
        }
    }
    input[j] = '\0';
}

/**
 * @brief Parses a .csv file and fills the Request struct.
 *
 * @note A valid .csv file follows these rules:
 *  1. A valid row is in the format of `W,Adr,Val` or `R,Adr` or `R,Adr,<whitespace(s)>`.
 *  2. New lines at the end of the file are allowed.
 *  3. New lines between valid rows are allowed.
 *  4. `Adr` can be either decimal or hexadecimal (e.g., 0x123 or 0X123).
 *  5. `Val` can be either decimal or hexadecimal (e.g., 0x123 or 0X123).
 *  6. The number of valid rows must be greater than or equal to the number of requests to be simulated.
 *
 * @param input_filename The name of the .csv file to parse.
 * @param requests An array of Request structs to fill.
 * @param numRequests The number of requests to be simulated.
 * @param customReq Read to the end of csv if false
 *
 * @return int (0 if successful, -1 if failed)
 *
 * @warning DO NOT REMOVE ANY OF THE COMMENTS!
 * @author Lie Leon Alexius
 */
int parse_csv(const char* input_filename, struct Request* requests, int numRequests, bool customReq) {
    // Open the file
    // Syntax: FILE* fptr; fptr = fopen(filename, mode);
    FILE* file = fopen(input_filename, "r");
    if (!file) {
        // No need to close file here since fopen will return NULL
        fprintf(stderr, "Failed to open input file\n");
        return -1;
    }

    // Initialize buffer
    /*
        __uint32_t max = 4_294_967_295 = 0xFFFFFFFF (10 char)
        Max 1 Line = 1 + 1 + 1 + 10 + 1 + 1 + 10 = 25 char
    */
    char line[100]; // assume a line up to 100 char (user is high on whitespaces)
    char rw[10]; // R or W
    char addr_str[20]; // Hexadecimal or Decimal
    char data_str[20]; // Hexadecimal or Decimal

    int i = 0; // request(s) counter

    // Read the requests from the file
    // Syntax: fgets(buffer, buffer_size, fptr);
    /*
        """
        my name is John Doe\n
        my name is Jane Doe\n
        \n
        \n
        """
        fgets() -> "my name is John Doe\n"
        fgets() -> "my name is Jane Doe\n"
        fgets() -> "\n"
        fgets() -> "\n"
    */
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove the first occurrence of (\n) replace it with the null character (\0)
        line[strcspn(line, "\n")] = 0;

        // Reset buffers
        memset(rw, 0, sizeof(rw));
        memset(addr_str, 0, sizeof(addr_str));
        memset(data_str, 0, sizeof(data_str));

        // Parse the line
        // %[^,] matches any sequence of characters except for a comma (stop if see comma)
        // , is a delimiter marks that next field starts after a comma
        int fields = sscanf(line, "%[^,],%[^,],%s", rw, addr_str, data_str);

        /*  Cases
            1. "Hello,World,!" -> "Hello"; "World"; "!"
            2. "He,llo,World,!" -> "He"; "llo"; "World,!" (PROBLEM)
            3. "    " -> "    "; ""; "" (EDGE CASE)
        */

        // Remove whitespace(s)
        remove_whitespaces(rw);
        remove_whitespaces(addr_str);
        remove_whitespaces(data_str);

        // Check if the third field has no other field
        // strchr() returns a pointer to the first occurrence of the character c in the string s
        if (strchr(data_str, ',') != NULL) {
            fprintf(stderr, "Invalid third collumn: %s\n", data_str);
            return -1;
        }
        
        // Case: trailing whitespace in the data field for read requests
        // Note: remove_whitespaces() should have made 1st char into '\0'
        if (fields == 3 && rw[0] == 'R' && data_str[0] == '\0') {
            fields = 2;
        }

        // Min. valid field in a row = 2
        // Note: This should be triggered only if the syntax of .csv is false
        //       We ignore Empty lines (look at if-else if-else)
        if (fields < 2 && (rw[0] != '\0' || addr_str[0] != '\0' || data_str[0] != '\0')) {
            fprintf(stderr, "Error in parsing the data - wrong format\n");
            return -1;
        }

        // Case: Write
        if (strcmp(rw, "W") == 0) {
            if (fields != 3) {
                fprintf(stderr, "Error in parsing the data - wrong format for write request\n");
                return -1;
            }
            requests[i].we = 1;
            
            // Parse address
            if (addr_str[1] == 'x' || addr_str[1] == 'X') {
                sscanf(addr_str, "%x", &requests[i].addr);
            } 
            else {
                sscanf(addr_str, "%u", &requests[i].addr);
            }

            // Parse data
            if (data_str[1] == 'x' || data_str[1] == 'X') {
                sscanf(data_str, "%x", &requests[i].data);
            } 
            else {
                sscanf(data_str, "%u", &requests[i].data);
            }

            i++;
        }

        // Case: Read
        else if (strcmp(rw, "R") == 0) {
            if (fields != 2) {
                fprintf(stderr, "Error in parsing the data - wrong format for read request\n");
                return -1;
            }

            requests[i].we = 0;
            requests[i].data = 0; // Default value for Read

            // Parse address
            if (addr_str[1] == 'x' || addr_str[1] == 'X') {
                sscanf(addr_str, "%x", &requests[i].addr);
            } 
            else {
                sscanf(addr_str, "%u", &requests[i].addr);
            }

            i++;
        }

        // Case: Unknown Op
        else {
            // ignore empty line with whitespaces (See case-3 in sscanf())
            if(rw[0] == '\0') {
                continue;
            }
            else {
                fprintf(stderr, "Error in parsing the data - unrecognized command\n");
                return -1;
            }
        }

        // Case: Enough valid Requests has been read
        if (customReq && i >= numRequests) {
            break;
        }
    }

    // Close the file
    fclose(file);

    // check if numRequests has been fulfilled
    if (customReq && i != numRequests) {
        fprintf(stderr, "Error: number of requests parsed does not match numRequests\n");
        return -1;
    }

    // safety: initialize request[i].we as invalid value to know when to stop
    requests[i].we = -1;

    return 0;
}
