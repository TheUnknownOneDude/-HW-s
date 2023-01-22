#include "math.h"
#include "return_codes.h"

#include <malloc.h>
#include <stdio.h>

void freeAll(float **a, int n)
{
	for (int i = 0; i < n; i++)
	{
		free(a[i]);
	}
	free(a);
}

int main(int argc, char *args[])
{
	if (argc != 3)
	{
		printf("Not enough program arguments");
		return ERROR_INVALID_PARAMETER;	   // слишком не столько сколько нужно аргументов
	}
	FILE *in = fopen(args[1], "r");

	if (in == NULL)
	{
		printf("No file with such name");
		return ERROR_FILE_NOT_FOUND;	// файла нет
	}

	float eps = 0.00001f;
	int n;
	int line = 0;
	int amount = fscanf(in, "%ull", &n);

	if (amount < 1)
	{
		printf("No elements in file");
		fclose(in);
		return ERROR_INVALID_DATA;
	}
	// выделяем память
	float **a = malloc(n * sizeof(float *));

	if (a == NULL)
	{
		printf("Not enough memory");
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	for (int i = 0; i < n; i++)
	{
		a[i] = malloc((n + 1) * sizeof(float));
		if (a[i] == NULL)
		{
			for (int j = 0; j < i; j++)
			{
				free(a[j]);
			}
			free(a);
			fclose(in);
			printf("Not enough memory");
			return ERROR_NOT_ENOUGH_MEMORY;
		}
	}

	// вводим матрицу
	amount = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n + 1; j++)
		{
			amount += fscanf(in, "%f", &a[i][j]);
		}
	}
	fclose(in);

	if (amount != n * (n + 1))
	{
		freeAll(a, n);
		printf("Not enough elements");
		return ERROR_INVALID_DATA;
	}

	int k = 0;
	// Находим строку с наибольшим a_i1
	for (int i = 1; i < n; i++)
	{
		if (fabsf(a[i][0]) > fabsf(a[k][0]))
		{
			k = i;
		}
	}
	// меняем местами строчку
	if (k > 0)
	{
		float swap1;
		for (int i = 0; i < n + 1; i++)
		{
			swap1 = a[0][i];
			a[0][i] = a[k][i];
			a[k][i] = swap1;
		}
	}

	while (line < n)
	{
		if (line != 0)
		{
			// Находим строку с наибольшим a_i|line по модулю
			for (int i = line; i < n; i++)
			{
				if (fabsf(a[i][line]) > fabsf(a[k][line]))
				{
					k = i;
				}
			}
			//			if (a[k][line] == 0)
			//			{
			//				nullCol++;
			//				continue;
			//			}
			// меняем местами да копипаста потом пофикшу
			if (k > line)
			{
				float swap1;
				for (int i = line; i < n + 1; i++)
				{
					swap1 = a[line][i];
					a[line][i] = a[k][i];
					a[k][i] = swap1;
				}
			}
		}
		// делим на a[i][line] в каждой строке
		for (int i = line; i < n; i++)
		{
			float delim = a[i][line];
			if (delim != 0)
			{
				for (int j = line; j < n + 1; j++)
				{
					a[i][j] /= delim;
				}
			}
		}

		// вычитаем линии
		for (int i = line + 1; i < n; i++)
		{
			int flag = 1;
			int count = 0;
			for (int j = 0; j < line + 1; j++)
			{
				if (a[i][j] == 0)
				{
					count++;	//  я забыл подчистую что я здесь считал, но это нужно.
				}
			}
			if (count == line + 1)
			{
				flag = 0;
			}
			if (flag == 1)
			{
				for (int j = line; j < n + 1; j++)
				{
					a[i][j] -= a[line][j];
					if (fabsf(a[i][j]) <= fabsf(eps))
					{
						a[i][j] = 0;
					}
				}
			}
		}
		line++;
	}
	FILE *out = fopen(args[2], "w");

	if (out == NULL)
	{
		printf("Can't create file");
		freeAll(a, n);
		return ERROR_FILE_NOT_FOUND;
	}

	for (int i = 0; i < n; i++)
	{
		int count = 0;
		for (int j = 0; j < n + 1; j++)
		{
			if (a[i][j] == 0)
			{
				count++;
			}
		}
		if (count == n + 1)
		{
			fprintf(out, "many solutions\n");

			freeAll(a, n);

			fclose(out);

			return ERROR_SUCCESS;
		}
		if (count == n && a[i][n] != 0)
		{
			fprintf(out, "no solution\n");

			freeAll(a, n);

			fclose(out);

			return ERROR_SUCCESS;
		}
	}

	// получаем ответ
	float *X;
	X = (float *)malloc(n * sizeof(float));

	if (X == NULL)
	{
		freeAll(a, n);
		fclose(out);
		printf("Not enough memory to make final computations");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	X[n - 1] = a[n - 1][n];
	for (int i = n - 2; i >= 0; i--)
	{
		for (int j = n - 1; j > i; j--)
		{
			a[i][n] -= a[i][j] * X[j];
		}
		X[i] = a[i][n];
	}

	// чистим память
	freeAll(a, n);

	for (int i = 0; i < n; i++)
	{
		fprintf(out, "%g\n", X[i]);
	}
	free(X);
	fclose(out);

	return ERROR_SUCCESS;
}