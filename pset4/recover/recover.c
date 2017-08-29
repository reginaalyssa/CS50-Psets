/**
 * Recovers JPEGs from a forensic image.
 *
 * Usage: recover image
 *
 * where image is the forensic image in which
 * the JPEGs will be recovered from.
 */

#include <stdio.h>

int main(int argc, char *argv[])
{
    // ensure correct usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    char *filename = argv[1];

    // open forensic image
    FILE *raw = fopen(filename, "r");
    if (raw == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", filename);
        return 2;
    }

    unsigned char buffer[512];
    int jpeg_count = 0;
    int first_jpeg_found = 0;
    FILE *img;

    // read 512 bytes until end of file
    while (fread(buffer, 512, 1, raw) != 0)
    {
        // JPEG is found as per its header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // first JPEG header in entire file
            if (first_jpeg_found == 0)
            {
                first_jpeg_found = 1;
            }
            else
            {
                // close previous JPEG file
                fclose(img);
            }

            // create and open new file to save JPEG to
            sprintf(filename, "%03i.jpg", jpeg_count);
            jpeg_count++;
            img = fopen(filename, "w");

            // write first 512 bytes to JPEG file
            fwrite(buffer, 512, 1, img);
        }
        // JPEG header was previously found already
        else if (first_jpeg_found == 1)
        {
            // write to currently opened JPEG file
            fwrite(buffer, 512, 1, img);
        }
    }
    // close forensic image
    fclose(raw);

    // success
    return 0;
}