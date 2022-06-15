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
#include "uncertain.h"
#include "userInput.h"
#include <stdio.h>
#include <stdlib.h>

/**
 *	@brief Insert measurement uncertainty into a buffer of values.
 *	@note Uncertainty is modelled with a uniform distribution here.
 *
 *	@param buffer      : Pointer to buffer
 *	@param uncertainty : measurement uncertainty (the support of the uniform distribution)
 */
static void
applyUncertainty(DoubleHeapBuffer * const buffer, const double uncertainty);

/**
 *	@brief Run dead reckoning calculation.
 *
 *	@param speedBuffer : Buffer containing uncertain speed measurement inputs
 *	@param timestep    : Time between successive speed measurements
 */
static void
deadReckoning(const DoubleHeapBuffer * const speedBuffer, const double timestep);

int
main(int argc, char * argv[])
{
	DoubleHeapBuffer speedBuffer = {
		.heapPointer = NULL,
		.size = 0,
	};
	CommandLineArguments arguments = {
		.speedInputFilePath = "speed.csv",
		.speedUncertainty = 1.0,
		.timestep = 0.1,
	};

	if (getCommandLineArguments(argc, argv, &arguments))
	{
		goto ERROR_EXIT_PROGRAM;
	}

	if (readFromCSVFile(&speedBuffer, arguments.speedInputFilePath))
	{
		goto ERROR_EXIT_PROGRAM;
	}

	applyUncertainty(&speedBuffer, arguments.speedUncertainty);

	deadReckoning(&speedBuffer, arguments.timestep);

	if (freeHeapBuffer(&speedBuffer))
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;

ERROR_EXIT_PROGRAM:
	(void)freeHeapBuffer(&speedBuffer);
	return EXIT_FAILURE;
}

static void
applyUncertainty(DoubleHeapBuffer * const buffer, const double uncertainty)
{
	for (size_t i = 0; i < buffer->size; i++)
	{
		const double value = buffer->heapPointer[i];
		const double lowerBound = value - uncertainty / 2.0;
		const double upperBound = value + uncertainty / 2.0;
		buffer->heapPointer[i] = libUncertainDoubleUniformDist(lowerBound, upperBound);
	}
}

static void
deadReckoning(const DoubleHeapBuffer * const speedBuffer, const double timestep)
{
	double position = 0.0;

	printf("time: %lf, position: %lf\n", 0.0, position);

	for (size_t i = 0; i < speedBuffer->size; i++)
	{
		position += timestep * speedBuffer->heapPointer[i];
		printf("time: %lf, position: %lf\n", (i + 1) * timestep, position);
	}
}
