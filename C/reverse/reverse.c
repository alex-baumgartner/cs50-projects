#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wav.h"

bool check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Read header
    // TODO #3
    WAVHEADER header;
    fread(&header, sizeof(header), 1, input);

    // Use check_format to ensure WAV format
    // TODO #4
    if (!check_format(header))
    {
        printf("Input file has incorrect format! \n");
        fclose(input);
        return 1;
    }

    // Open output file for writing
    // TODO #5
    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    // Write header to file
    // TODO #6
    fwrite(&header, sizeof(header), 1, output);
    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = get_block_size(header);
    // Write reversed audio to file
    // TODO #8
    fseek(input, block_size, SEEK_END);
    while(ftell(input) - block_size > sizeof(header))
    {
        fseek(input, -2 * block_size, SEEK_CUR);
        BYTE data[block_size];
        fread(&data, block_size, 1, input);
        fwrite(&data, block_size, 1, output);
    }
    fclose(input);
    fclose(output);
}

bool check_format(WAVHEADER header)
{
    for (int i = 0; i < 4; i++)
    {
        if (i == 0 && header.format[i] != 'W')
        {
            return false;
        }
        if (i == 1 && header.format[i] != 'A')
        {
            return false;
        }
        if (i == 2 && header.format[i] != 'V')
        {
            return false;
        }
        if (i == 3 && header.format[i] != 'E')
        {
            return false;
        }
    }
    return true;
}

int get_block_size(WAVHEADER header)
{
    int bitsPerSample = (int)header.bitsPerSample;
    int bytesPerSample = bitsPerSample / 8;
    return (int)header.numChannels * bytesPerSample;
}