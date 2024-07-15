// Lie Leon Alexius
// https://github.com/nothings/stb/blob/master/stb_image.h
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

#include "printer.h"

/**
 * @brief
 * Prints the layout of the simulator and its initial attributes.
 * 
 * @todo fix this
 * 
 * @warning don't remove this! It's used for the initial layout of the simulator
 * @author Lie Leon Alexius
 */
void print_layout(Config* config, Result* result) {
    if (config->prettyPrint) {
        printf(
            "Team 150 - Cache Simulator\n"
            "An Overview of our simulation:\n\n"
            "┌────────────────────────────────────────────────────────────────┐\n"
            "|                            Processor                           |\n"
            "| ┌────────────────────────────────────────────────────────────┐ |\n"
            "| | Max Simulated Cycles: %-36d | |\n"
            "| | Cache Line Size: %-41d | |\n"
            "| └────────────────────────────────────────────────────────────┘ |\n"
            "| ┌────────────────────────────────────────────────────────────┐ |\n"
            "| |                          L1 Cache                          | |\n"
            "| | Lines: %-8d            | Latency: %-7d              | |\n"
            "| | Read Hits: %-8zu        | Read Misses: %-7zu          | |\n"
            "| | Write Hits: %-8zu       | Write Misses: %-7zu         | |\n"
            "| └────────────────────────────────────────────────────────────┘ |\n"
            "| ┌────────────────────────────────────────────────────────────┐ |\n"
            "| |                          L2 Cache                          | |\n"
            "| | Lines: %-8d            | Latency: %-7d              | |\n"
            "| | Read Hits: %-8zu        | Read Misses: %-7zu          | |\n"
            "| | Write Hits: %-8zu       | Write Misses: %-7zu         | |\n"
            "| └────────────────────────────────────────────────────────────┘ |\n"
            "| Number of Requests Processed: %-32zu |\n"
            "└────────────────────────────────┬───────────────────────────────┘\n"
            "                                 ↓                                \n"
            "┌────────────────────────────────────────────────────────────────┐\n"
            "|                           Buffers                              |\n"
            "| ┌────────────────────────────────────────────────────────────┐ |\n"
            "| | Prefetch Buffer: %-10d  |                             | |\n"
            "| | Storeback Buffer: %-10d | Conditional: %-14d | |\n"
            "| └────────────────────────────────────────────────────────────┘ |\n"
            "└────────────────────────────────┬───────────────────────────────┘\n"
            "                                 ↓                                \n"
            "┌────────────────────────────────────────────────────────────────┐\n"
            "|                               RAM                              |\n"
            "| Memory Latency: %-46d |\n"
            "| Number of RAM Requests: %-38zu |\n"
            "| Number of Read Requests: %-37zu |\n"
            "| Number of Write Requests: %-36zu |\n"
            "└────────────────────────────────────────────────────────────────┘\n\n",
            config->cycles, 
            config->cacheLineSize,
            config->l1CacheLines, config->l1CacheLatency, 
            result->cacheStats->read_hits_L1, result->cacheStats->read_misses_L1,
            result->cacheStats->write_hits_L1, result->cacheStats->write_misses_L1,
            config->l2CacheLines, config->l2CacheLatency, 
            result->cacheStats->read_hits_L2, result->cacheStats->read_misses_L2,
            result->cacheStats->write_hits_L2, result->cacheStats->write_misses_L2,
            config->numRequests,
            config->prefetchBuffer, config->storebackBuffer, config->storebackBufferCondition,
            config->memoryLatency, 
            (result->cacheStats->read_misses_L2 + result->cacheStats->write_hits + result->cacheStats->write_misses),
            result->cacheStats->read_misses_L2, 
            (result->cacheStats->write_hits + result->cacheStats->write_misses)
        );
    }

    printf("Number of Cycles: %zu \n", result->cycles);
    printf("Number of Cache Hits: %zu   \n", result->hits);
    printf("Number of Cache Misses: %zu \n", result->misses);
    printf("Number of Gates: %zu  \n", result->primitiveGateCount);
}

// Variables for printing image: pixels, and ASCII (brightness levels)
// Use Pointer -> Pointer rather than static array
unsigned char** image_data; 
char ascii_chars[] = " .:-=+*#%@";

/**
 * @brief free the memory of the image_data
 * @param width the width of the image data
 * @param height the height of the image data
 * @author Lie Leon Alexius
 */
void free_image_data(int width, int height) {
    for (int i = 0; i < width; i++) {
        free(image_data[i]);
    }
    free(image_data);
}

/**
 * @brief Prints an image to the terminal
 * @param width the width of the image data
 * @param height the height of the image data
 * @warning image must be grayscale before calling this
 * @author Lie Leon Alexius
 */
void print_image(int width, int height) {
    // iterate through the image pixels
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
 * @brief load image -> print -> clean up
 * 
 * @details
 * This function loads the image into image_data array (loaded as grayscale)
 * 
 * @return
 * `0` if sucess, `1` if failure
 * 
 * @note Auto prints, auto free-up memory
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
        return 1;
    }

    // Best: width = 100, height = 50 (Bigger is better, but need to scroll - terminal too small)
    int new_width = 100;
    int new_height = 50;

    // Allocate memory for the resized image
    unsigned char *resized_img = (unsigned char*)malloc(new_width * new_height);

    // Resize the image
    stbir_resize_uint8_linear(img, width, height, 0, resized_img, new_width, new_height, 0, STBIR_1CHANNEL);
    
    // Allocate memory for the image_data array based on the new image size
    image_data = (unsigned char**)malloc(new_width * sizeof(unsigned char*));
    for (int i = 0; i < new_width; i++) {
        image_data[i] = (unsigned char*)malloc(new_height * sizeof(unsigned char));
    }

    // Copy the resized image into the image_data array
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            image_data[x][y] = resized_img[y * new_width + x];
        }
    }

    // free the loaded image and temp buffer
    stbi_image_free(img);
    free(resized_img);

    // Call the print_image function with the new image size
    print_image(new_width, new_height);

    // free the buffer
    free_image_data(new_width, new_height);

    return 0;
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

    if (!load_image("src/assets/images/Himeko.png")) {
        return 0;
    }

    return 1;
}
