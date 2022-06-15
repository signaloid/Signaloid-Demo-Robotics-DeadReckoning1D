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

#include "heapBuffer.h"
#include <stdio.h>
#include <stdlib.h>

int
allocateHeapBuffer(DoubleHeapBuffer * const heapBuffer, const size_t size)
{
	if (heapBuffer == NULL)
	{
		printf("Error: null pointer passed to allocateHeapBuffer()\n");
		return EXIT_FAILURE;
	}

	heapBuffer->heapPointer = calloc(size, sizeof(double));
	heapBuffer->size = size;

	if (heapBuffer->heapPointer == NULL)
	{
		printf("Error: The program ran out of heap memory. Try reducing the amount of "
		       "input data, or increasing the amount of available memory by selecting a "
		       "different core.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int
freeHeapBuffer(DoubleHeapBuffer * const heapBuffer)
{
	if (heapBuffer == NULL)
	{
		printf("Error: null pointer passed to freeHeapBuffer()\n");
		return EXIT_FAILURE;
	}

	free(heapBuffer->heapPointer);

	return EXIT_SUCCESS;
}
