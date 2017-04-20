/*
 * main.c
 *
 *  Created on: 17.04.2017
 *      Author: BSDBlack
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX_FILENAME_LENGTH 255

int main(void)
{
	char filename[MAX_FILENAME_LENGTH];
	int input, overall_size, program_size, pointer_dest, loop_count, loop_level;
	int *memory, *temp, *stack_ptr, **loop_ptr;
	FILE *inputfile = NULL;

#if WIN32
	setvbuf(stdout, NULL, _IONBF, 0);
#endif

	fputs("Insert Filename: ", stdout);
	fgets(filename, 255, stdin);
	filename[strlen(filename) - 1] = '\0';

	if(NULL == (inputfile = fopen(filename, "r")))
	{
		fputs("Error, could not open file\n", stdout);
		return 1;
	}

	memory = NULL; temp = NULL; overall_size = 0;

	while(EOF != (input = fgetc(inputfile)))
	{
		++overall_size;
		memory = (int *) realloc(memory, sizeof(int) * overall_size);

		if(NULL == memory)
		{
			fputs("Error, could not allocate memory\n", stdout);
			return 1;
		}
		*(memory+overall_size - 1) = input;
	}

	program_size = 1;
	loop_count = 0;
	loop_level = 0;
	pointer_dest = 0;
	loop_ptr = NULL;
	temp = memory;
	stack_ptr =(int *) calloc(program_size, sizeof(int));

	while(temp != (memory + overall_size))
	{
		switch(*temp)
		{
		case '>' :
			if((pointer_dest+1) >= program_size)
			{
				stack_ptr = (int *) realloc(stack_ptr, sizeof(int) * ++program_size);
				*(stack_ptr + program_size - 1) = 0;
			}
			++pointer_dest;
			break;
		case '<' :
			--pointer_dest;
			break;
		case '+' :
			++*(stack_ptr + pointer_dest);
			break;
		case '-' :
			--*(stack_ptr + pointer_dest);
			break;
		case '.' :
			fputc(*(stack_ptr + pointer_dest), stdout);
			break;
		case ',' :
			*(stack_ptr + pointer_dest) = fgetc(stdin);
			break;
		case '[' :
			loop_ptr = (int **) realloc(loop_ptr, sizeof(int *) * (loop_count+1));
			*(loop_ptr + loop_count++) = temp;
			if(0 == *(stack_ptr + pointer_dest))
			{
				while(loop_level > 0)
				{
					if('[' == *temp)
					{
						++loop_level;
					}
					else if (']' == *temp)
					{
						--loop_level;
					}
					++temp;
				}
			}
			break;
		case ']' :
			if(0 != *(stack_ptr + pointer_dest))
			{
				temp = *(loop_ptr + loop_count - 1);
				++temp;
			}
			else
			{
				--loop_count;
			}
			break;
		default:
			break;
		}

		++temp;
	}

	free(stack_ptr);
	free(memory);
	return 0;
}
