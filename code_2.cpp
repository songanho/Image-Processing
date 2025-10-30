#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

void main()
{
	FILE* fp;
	unsigned char** input, ** output;
	int i, j, k, p;
	double val;
	double mask[3][3];
	double ff;
	double** out;



	//Memory alloc
	input = (unsigned char**)malloc(sizeof(unsigned char*) * 512);
	for (i = 0; i < 512; i++)
		input[i] = (unsigned char*)malloc(sizeof(unsigned char) * 512);

	output = (unsigned char**)malloc(sizeof(unsigned char*) * 512);
	for (i = 0; i < 512; i++)
		output[i] = (unsigned char*)malloc(sizeof(unsigned char) * 512);

	out = (double**)malloc(sizeof(double*) * 512);
	for (i = 0; i < 512; i++)
		out[i] = (double*)malloc(sizeof(double) * 512);

	//File read
	fp = fopen("lena.img", "rb");
	for (i = 0; i < 512; i++)
	{
		fread(input[i], sizeof(unsigned char), 512, fp);
	}
	fclose(fp);

	//Zoom-out (downsampling) : 512x512 Lena image to 256x256 Lena image
	for (i = 0; i < 512; i++)
	{
		for (j = 0; j < 512; j++)
		{
			output[i][j] = input[i][j];
		}
	}


	for (i = 0;i < 512;i++) {
		for (j = 0;j < 512;j++) {
			ff = output[i][j] + 128 * cos(2 * M_PI * j / 11) + 128;
			if (ff > 255)ff = 255;
			else if (ff < 0)ff = 0;
			out[i][j] = (unsigned char)ff;
		}
	}


	for (i = 0; i < 512; i++) {
		for (j = 5; j < 507; j++) {
			ff = 0;
			for (k = -5; k <= 5; k++) {
				ff = ff + out[i][j+k];
			}
			ff = ff / 11.0;
			if (ff > 255) ff = 255;
			else if (ff < 0) ff = 0;
			output[i][j] = (unsigned char)ff;
		}
	}


	//File write
	fp = fopen("output2.bmp", "wb");

	for (i = 0; i < 512; i++)
	{
		fwrite(output[i], sizeof(unsigned char), 512, fp);
	}
	fclose(fp);

	//Memory free
	for (i = 0; i < 512; i++)
		free(input[i]);
	free(input);

	for (i = 0; i < 512; i++)
		free(output[i]);
	free(output);
}