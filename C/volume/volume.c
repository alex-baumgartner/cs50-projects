// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);
    uint8_t buffer[HEADER_SIZE];

        // Read 44 bytes from input file
    size_t bytes_read = fread(buffer, sizeof(uint8_t), HEADER_SIZE, input);
    if (bytes_read != HEADER_SIZE) {
        printf("Failed to read expected number of bytes.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    // Write bytes to output file
    size_t bytes_written = fwrite(buffer, sizeof(uint8_t), HEADER_SIZE, output);
    if (bytes_written != HEADER_SIZE) {
        printf("Failed to write expected number of bytes.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    fseek(input, sizeof(uint8_t) * HEADER_SIZE, SEEK_SET);

    int16_t sample;
    while(fread(&sample, sizeof(int16_t), 1, input) == 1)
    {
        sample = sample * factor;
        fwrite(&sample, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
