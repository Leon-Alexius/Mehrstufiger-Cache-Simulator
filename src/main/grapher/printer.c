// Lie Leon Alexius
// https://github.com/nothings/stb/blob/master/stb_image.h
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "printer.h"

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

// Variables for printing image: pixels, and ASCII (brightness levels)
unsigned char image_data[100][100];
char ascii_chars[] = " .:-=+*#%@";

/**
 * @brief Prints an image to the terminal
 * @warning image must be grayscale before calling this
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

/**
 * @brief This function loads the image into image_data array (loaded as grayscale)
 * @author Lie Leon Alexius
 */
int load_image(const char *filename) {
    int width, height, channels;
    /*
        1: Grayscale
        3: RGB
    */
    unsigned char *img = stbi_load(filename, &width, &height, &channels, 1); 

    if (img == NULL) {
        printf("Error loading image\n");
        return 0;
    }

    // Resize and copy the image into the image_data array (assuming the image is larger than 100x100)
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            image_data[x][y] = img[y * width + x];
        }
    }

    stbi_image_free(img);
    return 1;
}

/**
 * @brief Converts an RGB image to grayscale
 * 
 * @param img The input image data (RGB)
 * @param width The width of the image
 * @param height The height of the image
 * 
 * @return The grayscale image data
 * @warning not used actually
 * @author Lie Leon Alexius
 */
unsigned char* convert_rgb_to_grayscale(unsigned char* img, int width, int height) {
    unsigned char* grayscale_img = malloc(width * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            unsigned char r = img[index * 3];
            unsigned char g = img[index * 3 + 1];
            unsigned char b = img[index * 3 + 2];

            // Calculate the average of the RGB values
            grayscale_img[index] = (r + g + b) / 3;
        }
    }
    return grayscale_img;
}

/**
 * @brief prototype Test
 * @author Lie Leon Alexius
 */
int testPrinter() {
    if (!load_image("test.png")) {
        return 1;
    }
    
    print_image_to_terminal();

    return 0;
}
