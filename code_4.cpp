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
	double ff1, ff2;
	double** f;
	double max, min;

	f = (double**)malloc(sizeof(double*) * 512);
	for (i = 0;i < 512;i++) {
		f[i] = (double*)malloc(sizeof(double) * 512);
	}

	//Memory alloc
	input = (unsigned char**)malloc(sizeof(unsigned char*) * 512);
	for (i = 0; i < 512; i++)
		input[i] = (unsigned char*)malloc(sizeof(unsigned char) * 512);

	output = (unsigned char**)malloc(sizeof(unsigned char*) * 512);
	for (i = 0; i < 512; i++)
		output[i] = (unsigned char*)malloc(sizeof(unsigned char) * 512);

	//File read
	fp = fopen("lena.img", "rb");
	for (i = 0; i < 512; i++)
	{
		fread(input[i], sizeof(unsigned char), 512, fp);
	}
	fclose(fp);


	//Zoom-out (downsampling) : 512x512 Lena image to 256x256 Lena image

	
	mask[0][0] = 1.0; mask[0][1] = 2.0; mask[0][2] = 1.0;
	mask[1][0] = 0.0; mask[1][1] = 0.0; mask[1][2] = 0.0;
	mask[2][0] = -1.0; mask[2][1] = -2.0; mask[2][2] = -1.0;
	/*
	mask[0][0] = 1.0; mask[0][1] = 0.0; mask[0][2] = -1.0;
	mask[1][0] = 2.0; mask[1][1] = 0.0; mask[1][2] = -2.0;
	mask[2][0] = 1.0; mask[2][1] = 0.0; mask[2][2] = -1.0;
	*/

	for (i = 1; i < 511; i++) {
		for (j = 1; j < 511; j++) {
			f[i][j] = 0.0;

			for (k = -1; k <= 1; k++) {
				for (p = -1; p <= 1; p++) {
					f[i][j] = f[i][j] + (double)input[i + k][j + p] * mask[k + 1][p + 1];
				}	
			}
			f[i][j] = sqrt(f[i][j] * f[i][j]);
			if (f[i][j] > 255)f[i][j] = 255;
		}
	}


	for (i = 1;i < 511;i++) {
		for (j = 1;j < 511;j++) {
			output[i][j] = (unsigned char)f[i][j];
		}
	}

	//File write
	fp = fopen("output4.bmp", "wb");

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

	for (i = 0; i < 512; i++)
		free(f[i]);
	free(f);
}