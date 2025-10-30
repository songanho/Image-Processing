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
	double sum_real, sum_img, theta;
	double** R;
	double** I;
	double** F;
	double** BF;
	double** IR;
	double** II;

	R = (double**)malloc(sizeof(double*) * 256);
	for (i = 0;i < 256;i++)
		R[i] = (double*)malloc(sizeof(double) * 256);
	I = (double**)malloc(sizeof(double*) * 256);
	for (i = 0;i < 256;i++)
		I[i] = (double*)malloc(sizeof(double) * 256);
	F = (double**)malloc(sizeof(double*) * 256);
	for (i = 0;i < 256;i++)
		F[i] = (double*)malloc(sizeof(double) * 256);
	BF = (double**)malloc(sizeof(double*) * 256);
	for (i = 0;i < 256;i++)
		BF[i] = (double*)malloc(sizeof(double) * 256);
	IR = (double**)malloc(sizeof(double*) * 256);
	for (i = 0;i < 256;i++)
		IR[i] = (double*)malloc(sizeof(double) * 256);
	II = (double**)malloc(sizeof(double*) * 256);
	for (i = 0;i < 256;i++)
		II[i] = (double*)malloc(sizeof(double) * 256);

	//Memory alloc
	input = (unsigned char**)malloc(sizeof(unsigned char*) * 512);
	for (i = 0; i < 512; i++)
		input[i] = (unsigned char*)malloc(sizeof(unsigned char) * 512);

	output = (unsigned char**)malloc(sizeof(unsigned char*) * 256);
	for (i = 0; i < 256; i++)
		output[i] = (unsigned char*)malloc(sizeof(unsigned char) * 256);

	//File read
	fp = fopen("lena.img", "rb");
	for (i = 0; i < 512; i++)
	{
		fread(input[i], sizeof(unsigned char), 512, fp);
	}
	fclose(fp);

	//Zoom-out (downsampling) : 512x512 Lena image to 256x256 Lena image
	for (i = 0; i < 512; i += 2)
	{
		for (j = 0; j < 512; j += 2)
		{
			val = (input[i][j] + input[i + 1][j] + input[i][j + 1] + input[i + 1][j + 1]) / 4;
			output[i / 2][j / 2] = (unsigned char)val;
		}
	}

	for (i = 0;i < 256;i++) {
		for (j = 0;j < 256;j++) {
			output[i][j] = pow(-1, i + j) * output[i][j];
		}
	}


	for (i = 0;i < 256;i++) {
		for (j = 0;j < 256;j++) {
			sum_real = 0;
			sum_img = 0;
			for (k = 0;k < 256;k++) {
				for (p = 0;p < 256;p++) {
					theta = 2.0 * M_PI * ((double)(i * k) / 256.0 + (double)(j * p) / 256.0);
					sum_real += output[k][p] * cos(-1 * theta);
					sum_img += output[k][p] * sin(-1 * theta);
				}
			}

			R[i][j] = sum_real / (double)(256 * 256);
			I[i][j] = sum_img / (double)(256 * 256);
		}
	}

	

	for (i = 0;i < 256;i++) {
		for (j = 0;j < 256;j++) {
			F[i][j] = sqrt(R[i][j] * R[i][j] + I[i][j] * I[i][j]);
		}
	}
	for (i = 0;i < 256;i++) {
		for (j = 0;j < 256;j++) {
			BF[i][j] = 255 * log10(F[i][j] + 1.0) / log10(F[128][128] + 1.0);
			output[i][j] = (unsigned char)BF[i][j];
		}
	}

	fp = fopen("output_DFT-magnitude.bmp", "wb");

	for (i = 0; i < 256; i++)
	{
		fwrite(output[i], sizeof(unsigned char), 256, fp);
	}
	fclose(fp);


	for (i = 0;i < 256;i++) {
		for (j = 0;j < 256;j++) {
			sum_real = 0;
			sum_img = 0;
			for (k = 0;k < 256;k++) {
				for (p = 0;p < 256;p++) {
					theta = 2.0 * M_PI * ((double)(i * k) / 256.0 + (double)(j * p) / 256.0);
					sum_real += R[k][p] * cos(theta) - I[k][p] * sin(theta);
					sum_img += R[k][p] * sin(theta) + I[k][p] * cos(theta);
				}
			}

			IR[i][j] = sum_real + 0.49999;
			II[i][j] = sum_img + 0.499999;


			if (IR[i][j] > 255)IR[i][j] = 255;
			else if (IR[i][j] < 0)IR[i][j] = 0;
		}
	}

	for (i = 0;i < 256;i++) {
		for (j = 0;j < 256;j++) {
			IR[i][j] = pow(-1, i + j) * IR[i][j];
		}
	}

	for (i = 0;i < 256;i++) {
		for (j = 0;j < 256;j++) {
			output[i][j] = (unsigned char)IR[i][j];
		}
	}



	//File write
	fp = fopen("output_inverseDFT.bmp", "wb");

	for (i = 0; i < 256; i++)
	{
		fwrite(output[i], sizeof(unsigned char), 256, fp);
	}
	fclose(fp);

	//Memory free
	for (i = 0; i < 512; i++)
		free(input[i]);
	free(input);

	for (i = 0; i < 256; i++)
		free(output[i]);
	free(output);
}