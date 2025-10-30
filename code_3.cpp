#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
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
			out[i][j] = (double)output[i][j];
		}
	}

	mask[0][0] = 1.0 / 9.0; mask[0][1] = 1.0 / 9.0; mask[0][2] = 1.0 / 9.0;
	mask[1][0] = 1.0 / 9.0; mask[1][1] = 1.0 / 9.0; mask[1][2] = 1.0 / 9.0;
	mask[2][0] = 1.0 / 9.0; mask[2][1] = 1.0 / 9.0; mask[2][2] = 1.0 / 9.0;

	for (i = 1;i < 511;i++) {
		for (j = 1;j < 511;j++) {
			ff = 0;
			for (k = -1;k <= 1;k++) {
				for (p = -1;p <= 1;p++) {
					ff = ff + mask[k + 1][p + 1] * out[i + k][j + p];
				}
			}
			if (ff > 255)ff = 255;
			else if (ff < 0)ff = 0;
			output[i][j] = (unsigned char)ff;
		}
	}


	//File write
	fp = fopen("output3.bmp", "wb");

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