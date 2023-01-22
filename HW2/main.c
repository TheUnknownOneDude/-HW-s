#include "return_codes.h"
#include "stdbool.h"

#include <malloc.h>
#include <stdio.h>

#include <zlib.h>

unsigned char PaethPredictor(unsigned char a, unsigned char b, unsigned char c)
{
	int p = a + b - c;
	int pa = abs(p - a), pb = abs(p - b), pc = abs(p - c);
	if (pa <= pb && pa <= pc)
	{
		return a;
	}
	else if (pb <= pc)
	{
		return b;
	}
	else
	{
		return c;
	}
}

struct IHDR_chunk
{
	//    unsigned char length[4];
	unsigned char name[4];
	unsigned char width[4];
	unsigned char height[4];
	unsigned char colorDeep[1];
	unsigned char colorType[1];
	unsigned char deflate[1];
	unsigned char filter[1];
	unsigned char stroke[1];
	unsigned char CRC32[4];
	// 25 bytes
};

unsigned long int toBase(unsigned char *data, int k)
{
	int res = 0;
	int base = UCHAR_MAX + 1;
	int curBit = 1;
	for (int i = k - 1; i >= 0; i--)
	{
		res += data[i] * (curBit);
		curBit *= base;
	}
	return res;
}

int readFile(FILE *image, unsigned char *data, int k)
{
	int j = 0;
	for (int i = 0; i < k; i++)
	{
		data[i] = getc(image);
		j++;
	}
	return j;
}

void read_IHDR_chunk(FILE *image, struct IHDR_chunk *IHDR)
{
	//    readFile(image, IHDR.length, 4);
	//    readFile(image, IHDR.name, 4);
	readFile(image, IHDR->width, 4);
	readFile(image, IHDR->height, 4);
	readFile(image, IHDR->colorDeep, 1);
	readFile(image, IHDR->colorType, 1);
	readFile(image, IHDR->deflate, 1);
	readFile(image, IHDR->filter, 1);
	readFile(image, IHDR->stroke, 1);
	readFile(image, IHDR->CRC32, 4);
}

bool isSignature(const unsigned char *signatureInfo)
{
	return signatureInfo[0] == 137 && signatureInfo[1] == 'P' && signatureInfo[2] == 'N' && signatureInfo[3] == 'G' &&
		   signatureInfo[4] == '\r' && signatureInfo[5] == '\n' && signatureInfo[6] == '\032' && signatureInfo[7] == '\n';
}

bool isIDAT(const unsigned char *name)
{
	return name[0] == 'I' && name[1] == 'D' && name[2] == 'A' && name[3] == 'T';
}

bool isPLTE(const unsigned char *name)
{
	return name[0] == 'P' && name[1] == 'L' && name[2] == 'T' && name[3] == 'E';
}

void showChars(unsigned char *data, int k)
{
	for (int i = 0; i < k; i++)
	{
		printf("%c", data[i]);
	}
	printf("\n");
}

void showNums(unsigned char *data, int k)
{
	for (int i = 0; i < k; i++)
	{
		printf("%d", data[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "INVALID PARAMETERS");
		return ERROR_INVALID_PARAMETER;
	}
	FILE *image = fopen(argv[1], "rb");

	if (image == NULL)
	{
		fprintf(stderr, "File is not open");
		return ERROR_FILE_NOT_FOUND;
	}

	int read;

	unsigned char signatureInfo[8];

	if ((read = readFile(image, signatureInfo, 8) != 8))
	{
		fprintf(stderr, "WRONG DATA");
		fclose(image);
		return ERROR_INVALID_DATA;
	}

	if (!isSignature(signatureInfo))
	{
		fclose(image);
		fprintf(stderr, "Wrong signature");
		return ERROR_INVALID_DATA;
	}

	struct IHDR_chunk IHDR;
	unsigned char c;

	int k = 0;
	while (k != 4)
	{
		c = getc(image);
		if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')
			IHDR.name[k++] = c;
	}

	if (IHDR.name[0] != 'I' || IHDR.name[1] != 'H' || IHDR.name[2] != 'D' || IHDR.name[3] != 'R')
	{
		fclose(image);
		fprintf(stderr, "Wrong data");
		return ERROR_INVALID_DATA;
	}

	struct IHDR_chunk *ihdr = &IHDR;

	read_IHDR_chunk(image, ihdr);

	if (toBase(ihdr->deflate, 1) != 0)
	{
		fclose(image);
		fprintf(stderr, "Wrong deflate type");
		return ERROR_INVALID_DATA;
	}

	if (toBase(ihdr->filter, 1) != 0)
	{
		fclose(image);
		fprintf(stderr, "Wrong filter type");
		return ERROR_INVALID_DATA;
	}

	if (toBase(ihdr->stroke, 1) > 1)
	{
		fclose(image);
		fprintf(stderr, "Wrong stroke type");
		return ERROR_INVALID_DATA;
	}

	unsigned long int w = toBase(IHDR.width, 4);
	unsigned long int h = toBase(IHDR.height, 4);
	unsigned long int color = toBase(IHDR.colorType, 1);

	unsigned char length[4];
	unsigned char name[4];
	readFile(image, length, 4);

	readFile(image, name, 4);

	while (!isIDAT(name))
	{
		for (int i = 0; i < toBase(length, 4) + 4; i++)
		{
			c = getc(image);
		}

		readFile(image, length, 4);

		readFile(image, name, 4);

		if (isPLTE(name))
		{
			fprintf(stderr, "UNSUPPORTED CHUNK");
			return ERROR_UNSUPPORTED;
		}
	}

	unsigned char *resData;
	uLong dataSize = 0;
	unsigned char *curData;

	while (isIDAT(name))
	{
		unsigned long int curDataSize = toBase(length, 4);

		if (dataSize == 0)
		{
			resData = malloc(curDataSize * sizeof(unsigned char));
		}
		else
		{
			resData = realloc(resData, dataSize + curDataSize);
		}

		if (resData == NULL)
		{
			fprintf(stderr, "NOT ENOUGH MEMORY");
			free(resData);
			free(curData);
			return ERROR_OUTOFMEMORY;
		}

		curData = malloc(curDataSize * sizeof(unsigned char));

		if (curData == NULL)
		{
			fprintf(stderr, "NOT ENOUGH MEMORY");
			free(resData);
			free(curData);
			return ERROR_OUTOFMEMORY;
		}

		read = readFile(image, curData, curDataSize);

		if (read != curDataSize)
		{
			fprintf(stderr, "NOT ENOUGH MEMORY");
			free(resData);
			free(curData);
			return ERROR_INVALID_DATA;
		}

		memcpy(resData + dataSize, curData, curDataSize);

		dataSize += curDataSize;

		readFile(image, length, 4);	   // skip CRC32

		readFile(image, length, 4);

		readFile(image, name, 4);


	}

	fclose(image);
	free(curData);

	uLong bytesAmount = h * w * 2 + h;
	uLong *p = &bytesAmount;

	unsigned char *buffer;

	buffer = malloc(bytesAmount);

	if (buffer == NULL)
	{
		printf("BUFFER ERROR");
		free(buffer);
		free(resData);
		return ERROR_OUTOFMEMORY;
	}

	int error = uncompress(buffer, &bytesAmount, resData, dataSize);

	free(resData);

	if (error != Z_OK)
	{
		if (error == Z_DATA_ERROR)
		{
			fprintf(stderr, "WRONG DATA");
		}
		else if (error == Z_BUF_ERROR)
		{
			fprintf(stderr, "BUFFER ERROR");
		}
		free(buffer);
		return ERROR_UNSUPPORTED;
	}

	FILE *out = fopen(argv[2], "wb");

	if (out == NULL)
	{
		fprintf(stderr, "FILE NOT FOUND");
		free(buffer);
		return ERROR_FILE_NOT_FOUND;
	}

	if (color == 0)
	{
		fprintf(out, "P5\n%d %d\n65535\n", w, h);
	}
	else if (color == 2)
	{
		fprintf(out, "P6\n%d %d\n255\n", w, h);
	}
	else
	{
		fprintf(stderr, "UNSUPPORTED FUNCTIONAL");
		free(buffer);
		fclose(out);
		return ERROR_UNSUPPORTED;
	}

	int bpp = color == 0 ? 1 : 3;
	bpp = 1;
	int mappedWidth = bpp * w + 1;
	printf("bytes amount is: %i\n", bytesAmount);
	unsigned char curMode;
	short *rofl;
	rofl = malloc(bytesAmount * sizeof(short ));
	for (int i = 0; i < bytesAmount; i += mappedWidth)
	{
		curMode = buffer[i];
		for (int j = 1; j < mappedWidth; j++)
		{
			if (curMode == 1 && j > bpp)
			{
				buffer[i + j] += buffer[i + j - bpp];
			}
			else if (curMode == 2 && i > 0)
			{
				buffer[i + j] += buffer[i - mappedWidth + j];
			}
			else if (curMode == 3 && i > 0 && j > bpp)
			{
				buffer[i + j] += ((int)buffer[i - mappedWidth + j] + (int)buffer[i + j - bpp]) / 2;
			}
			else if (curMode == 3 && i > 0 && j <= bpp)
			{
				buffer[i + j] += ((int)buffer[i - mappedWidth + j]) / 2;
			}
			else if (curMode == 4 && i > 0 && j > bpp)
			{
				buffer[i + j] +=
					PaethPredictor(buffer[i + j - bpp], buffer[i - mappedWidth + j], buffer[i - mappedWidth + j - bpp]);
			}

			else if (curMode == 4 && i > 0 && j <= bpp)
			{
				buffer[i + j] += buffer[i - mappedWidth + j];
			}
			else if (curMode == 4)
			{
				buffer[i + j] += buffer[i + j - bpp];
			}
		}
	}

	for (int i = 0; i < bytesAmount; i++) {
		if (i % mappedWidth == 0) continue;

		rofl[i] = buffer[i];
	}

	for (int i = 0; i < h; ++i)
	{
//		printf("%i\n", (i * mappedWidth) + 1);
		fwrite(rofl + (i * mappedWidth) + 1, sizeof (short), mappedWidth - 1, out);
	}

	return ERROR_SUCCESS;
}
