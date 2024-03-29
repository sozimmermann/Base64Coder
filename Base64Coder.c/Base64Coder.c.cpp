// Base64Coder.c.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>

#include "stdafx.h"
#include <stdint.h>
#include <stdlib.h>

/*see Base64 encoding*/
char* mask_bits(unsigned char char_bits[3], char* pointer)
{
	unsigned char tmp = 0;
	unsigned char token[4] = { 0 };


	token[0] = char_bits[0] & 0b11111100;
	token[0] >>= 2;

	token[1] = (char_bits[0] & 0b00000011) << 4;
	tmp = char_bits[1] & 0b11110000;
	tmp >>= 4;
	token[1] = token[1] | tmp;


	token[2] = char_bits[1] & 0b00001111;
	token[2] <<= 2;
	tmp = char_bits[2] & 0b11000000;
	tmp >>= 6;
	token[2] = token[2] | tmp;

	token[3] = char_bits[2] & 0b00111111;

	for (int x = 0; x <= 3; x++)
	{
		if (token[x] >= 0 && token[x] <= 25) {
			token[x] += 65;
		}
		else if (token[x] >= 26 && token[x] <= 51) {
			token[x] += 71;
		}
		else if (token[x] >= 52 && token[x] <= 61) {
			token[x] -= 4;
		}
		else if (token[x] == 62){
			token[x] = 43;
		}
		else if (token[x] == 63){
			token[x] = 47;
		}
		else {
			token[x] = '=';
		}
		*pointer = token[x];
		pointer++;
	}

	return pointer;
}


char* write_to_txt_file(char* pointer) {

	FILE *f = fopen("D:\\base_decoded.txt", "w");
	/*	
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	*/

	fprintf(f, "%s", pointer);
	fclose(f);

	return pointer;
}

int main()
{
	FILE *fpointer = NULL;							// pointer open original image
	char *pointer = NULL;							// pointer encoded file (output)
	char *pointer_current_position = NULL;			// pointer encoded file (output) at current position while writing
	char *input_pointer_puffer = NULL;				// pointer original image puffer save 
	int size_input_file = 0;						//
	int size = 0;									// calculated size for pointer encoded file (output)
	int j = 0;										// counter while lower size of image 
	unsigned char temp_char[3] = { 0 };				// 3 Bytes per transformation (3*8)/ 6 == possible 

	fpointer = fopen("C:\\Users\\Koro\\Pictures\\town.png", "rb");

	if (fpointer == NULL)
	{
		printf("Error in opening the image");
		fclose(fpointer);
		exit(0);
	}

	fseek(fpointer, 0L, SEEK_END);
	size_input_file = ftell(fpointer) + 2;
	rewind(fpointer);

	size = 4 * ((size_input_file + 2 - ((size_input_file + 2) % 3)) / 3) + 20 ;			// calc puffer
	pointer = (char*)malloc(size);

	input_pointer_puffer = (char*)malloc(size_input_file);


	fread(input_pointer_puffer, 1, size_input_file, fpointer);
	pointer_current_position = pointer;


	printf("Successfully opened image");
	

	while (j <= size_input_file)
	{
		for (int i = 0; i < 3; i++)
		{
			temp_char[i] = input_pointer_puffer[j];		// 3 Bytes to Convert
		j++;
		}
		if (j >= size_input_file)						// if end of file, fill up with '=' 
		{						
			*pointer_current_position = '=';
			break;
		}
			
		pointer_current_position = mask_bits(temp_char, pointer_current_position);
	
	}

	
	printf("\nCoding completed");

	write_to_txt_file(pointer);

	fclose(fpointer);
	system("pause");
	return 0;

}


