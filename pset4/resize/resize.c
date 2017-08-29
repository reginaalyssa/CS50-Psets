/**
 * Resizes a BMP by a factor of n.
 *
 * Usage: resize n infile outfile
 *
 * where n is a positive integer with max value of 100,
 * infile is the original BMP to be resizedm
 * and outfile is where the resized BMP will be written to.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // check for characters that are not a number
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            fprintf(stderr, "n must be an integer\nUsage: ./resize n infile outfile\n");
            return 1;
        }
    }

    // check if integer n is within range
    int n = atoi(argv[1]);
    if (n <= 0 || n > 100)
    {
        fprintf(stderr,
                "n must be a positive integer less than or equal to 100\nUsage: ./resize n infile outfile\n");
        return 1;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // store old values of biWidth and biHeight
    LONG old_biWidth = bi.biWidth;
    LONG old_biHeight = bi.biHeight;

    // compute for new values of biWidth and biHeight
    bi.biWidth *= n;
    bi.biHeight *= n;

    // determine old padding
    int old_padding = (4 - (old_biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine new padding for scanlines
    int new_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // compute new biSizeImage and new bfSize
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + new_padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(old_biHeight); i < biHeight; i++)
    {
        // repeat row printing n times
        for (int j = 0; j < n; j++)
        {
            // iterate over pixels in scanline
            for (int k = 0; k < old_biWidth; k++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile n times
                for (int l = 0; l < n; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip over old padding, if any
            fseek(inptr, old_padding, SEEK_CUR);

            // then add new padding to outfile
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }

            // go back to start of row if not the last iteration
            if (j < n - 1)
            {
                fseek(inptr, -old_padding, SEEK_CUR);
                fseek(inptr, -(long int)(sizeof(RGBTRIPLE) * old_biWidth), SEEK_CUR);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
