#ifndef PRINTER_H
#define PRINTER_H

// Variables for printing image, pixels, and ASCII (brightness levels)
extern unsigned char image_data[100][100];
extern char ascii_chars[];

// Prototypes
void print_layout();
void print_image_to_terminal();
int load_image(const char *filename);

#endif // PRINTER_H