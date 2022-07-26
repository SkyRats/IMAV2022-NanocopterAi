#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char* readPPM(const char* const fileName, unsigned int* const width, unsigned int* const height, unsigned int* const maximum)
{
	const char p5[] = "P5";
	char header[10];
	unsigned char *pixels = NULL;

	*width = *height = *maximum = 0;

	FILE* fr = fopen(fileName, "rb");
	if (fr == NULL) {
		printf("Unable to open file %s\n", fileName);
		return NULL;
	}

	fscanf(fr, "%9s", header);
	if (strcmp(header, p5))
		printf("Incorrect header %s found\n", header);
	else
		if (fscanf(fr, "%d %d\n%d\n", width, height, maximum) != 3)
			puts("Invalid header values");
		else {
			const unsigned int size = *width * *height;

			if ((pixels = malloc(size)) == NULL)
				puts("Error allocating memory");
			else {
				unsigned int read = 0;

				if ((read = fread(pixels, 1, size, fr)) != size) {
					printf("Error reading binary data. Wanted %i, got %i\n", size, read);
					free(pixels);
					pixels = NULL;
                }
			}
		}

	fclose(fr);
	return pixels;
}

int main()
{
	//char fileName[50] = "binary_pixels.txt";
	char fileName[50] = "../images/img.ppm";
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int maximum = 0;

	unsigned char* pixelArray = readPPM(fileName, &width, &height, &maximum);

	if (pixelArray) {
		printf("Width: %d\n", width);
		printf("Height: %d\n", height);
		printf("maximum: %d\n", maximum);
		free(pixelArray);
	}

    return 0;
}
