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

#pragma once

#include "heapBuffer.h"

typedef struct CommandLineArguments
{
	char * speedInputFilePath;
	double speedUncertainty;
	double timestep;
} CommandLineArguments;

/**
 *	@brief Get command line arguments.
 *
 *	@param argc      : Argument count from main()
 *	@param argv      : Argument vector from main()
 *	@param arguments : Pointer to struct to store arguments
 *	@return int : EXIT_SUCCESS if successful, else EXIT_FAILURE
 */
int
getCommandLineArguments(int argc, char * argv[], CommandLineArguments * const arguments);

/**
 *	@brief Read data from a CSV file into a heap buffer.
 *
 *	@param buffer   : Pointer to buffer to store data
 *	@param filePath : Path to CSV file
 *	@return int : EXIT_SUCCESS if successful, else EXIT_FAILURE
 */
int
readFromCSVFile(DoubleHeapBuffer * const buffer, const char * const filePath);
