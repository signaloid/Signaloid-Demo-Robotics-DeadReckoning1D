/*
 *	Authored 2022, Greg Brooks.
 *
 *	Copyright (c) 2022, Signaloid.
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#include "userInput.h"
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

extern char * optarg;
extern int    opterr;

static void
printUsage(void)
{
	printf("Example demonstrating dead reckoning in one dimension.\n\n");
	printf("Usage is:\n");
	printf("dead-reckoning-1d\n"
	       "[-s path to speed input data CSV file]\n"
	       "[-u measurement uncertainty for speed data]\n"
	       "[-t measurement time period]\n"
	       "[-h show this help message]\n"
	       "\n");
}

/**
 *	@brief Determine the size of array required to store values from a CSV file.
 *
 *	@param stream : File stream to read from.
 *	@return size_t : Size of array required to store values from the CSV file.
 */
static size_t
determineBufferSize(FILE * stream)
{
	double buf;
	int    fscanfResult;
	size_t count = 0;

	do
	{
		fscanfResult = fscanf(stream, "%lf,", &buf);
		count++;
		if (count == 0)
		{
			/*
			 * Overflow
			 */
			break;
		}
	} while (fscanfResult != EOF && fscanfResult != 0);

	rewind(stream);

	return --count;
}

int
getCommandLineArguments(int argc, char * argv[], CommandLineArguments * const arguments)
{
	int opt;

	if (argv == NULL || arguments == NULL)
	{
		printf("Error: null pointer passed to getCommandLineArguments()\n");
		return EXIT_FAILURE;
	}

	opterr = 0;

	while ((opt = getopt(argc, argv, ":s:u:t:h")) != EOF)
	{
		switch (opt)
		{
		case 's':
			arguments->speedInputFilePath = optarg;
			break;
		case 'u':
			arguments->speedUncertainty = atof(optarg);
			break;
		case 't':
			arguments->timestep = atof(optarg);
			if (arguments->timestep == 0.0)
			{
				printf("Error: invalid value for timestep\n");
				printUsage();
				return EXIT_FAILURE;
			}
			break;
		case 'h':
			printUsage();
			return EXIT_FAILURE;
		default:
			printf("Error: invalid option -%c\n", opt);
			printUsage();
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int
readFromCSVFile(DoubleHeapBuffer * const buffer, const char * const filePath)
{
	FILE * stream;

	if (buffer == NULL || filePath == NULL)
	{
		printf("Error: null pointer passed to readFromCSVFile()\n");
		return EXIT_FAILURE;
	}

	stream = fopen(filePath, "r");

	if (stream == NULL)
	{
		printf("Error: could not open file at path: %s\n", filePath);
		return EXIT_FAILURE;
	}

	buffer->size = determineBufferSize(stream);

	if (buffer->size == 0)
	{
		printf("Error: no data found in the specified file:'%s'\n", filePath);
		goto RETURN_FAILURE;
	}

	if (allocateHeapBuffer(buffer, buffer->size))
	{
		goto RETURN_FAILURE;
	}

	for (size_t i = 0; i < buffer->size; i++)
	{
		const int returnValue = fscanf(stream, "%lf,", &(buffer->heapPointer[i]));
		if (returnValue == EOF || returnValue == 0)
		{
			goto RETURN_FAILURE;
		}
	}

	fclose(stream);
	return EXIT_SUCCESS;

RETURN_FAILURE:
	fclose(stream);
	return EXIT_FAILURE;
}
