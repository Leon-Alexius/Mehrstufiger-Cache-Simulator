// Lie Leon Alexius


/**
 * @brief
 * Prints the layout of the simulator and its initial attributes.
 * 
 * @warning don't remove this!
 * @todo complete this
 * @author Lie Leon Alexius
 */
void print_layout() {
    printf(
        "Team 150 - Cache Simulator\n"
        "An Overview of our simulation:\n\n"
        "┌─────────────────┐\n"
        "|    Processor    |\n"
        "| ┌─────┐ ┌─────┐ |\n"
        "| | L1  | | L2  | |\n"
        "| └─────┘ └─────┘ |\n"
        "└────────┬────────┘\n"
        "         ↓         \n"
        "┌─────────────────┐\n"
        "|       RAM       |\n"
        "└─────────────────┘\n\n"
        "Attribute values:  \n"
    );
}

// Variables for printing image, pixels and ASCII (brightness levels)
unsigned char image_data[100][100];
char ascii_chars[] = " .:-=+*#%@";

/**
 * @brief print an image to the terminal
 * @warning image must be grayscale
 * @author Lie Leon Alexius
 */
void print_image_to_terminal() {
    int width = 100;
    int height = 100;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char pixel_value = image_data[x][y];

            // Map the pixel value to an index in the ascii_chars array.
            int ascii_index = pixel_value / (256 / (sizeof(ascii_chars) - 1));

            printf("%c", ascii_chars[ascii_index]);
        }

        printf("\n");
    }
}
