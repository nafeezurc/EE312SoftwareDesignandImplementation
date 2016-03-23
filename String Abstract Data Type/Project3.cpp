/* Project3.cpp
* Gopika Ajaykumar
* EE312, Spring 2016
* Project 3, String ADT
*/

// Included Files
#include <assert.h>
#include <string.h>
#include "MemHeap.h"
#include "String.h"

// Macro
#define SIGNATURE (~0xdeadbeef)
#define STRING(s) (((String*)s)-1)	// Unused

// Unused
int32_t isOurs(char* s) {
    if (!isHeapVar(s)) { return 0; }
    if (STRING(s)->check == SIGNATURE) { return 1; }
    else { return 0; }
}

/**************************************************************************************************
                                         utstrdup
This function allocates a utstring src on the heap and initializes the string correctly by copying
the characters from src. The function returns a pointer to the first character of actual string
data.
**************************************************************************************************/
char* utstrdup(const char* src)
{
	// Find the number of characters in the source.
	uint32_t size = 0;			// This variable holds the number of characters in the src array.
	while (src[size] != '\0')	// Note that we are using size as an index for the loop.
	{
		size += 1;
	}

	// Create a String struct on the heap. newStruct points to the created String struct.
	uint32_t* newStruct = (uint32_t*)malloc(3 * sizeof(uint32_t) + size + 1);	
	char* newString = (char *)(newStruct + 3);		// newString points to the string in the 
													// new struct we are creating

	// Initialize the new struct.
	*newStruct = SIGNATURE;			//	Signature
	*(newStruct + 1) = size;		// The capacity is the size of src.
	*(newStruct + 2) = size;		// The size of the string is the size of src.
	// Copy the src string to the new string. 
	for (uint32_t index = 0; index <= size; index += 1)
	{
		newString[index] = src[index];
	}

	// Return the address of the first character in the new string.
	return newString;
}

/**************************************************************************************************
                                    utstrlen
This function takes a utstring utstr and finds and returns the length of the string by accessing 
its metadata.
**************************************************************************************************/
uint32_t utstrlen(const char* utstr) 
{
	uint32_t* structPtr = (uint32_t*)utstr - 3;	// Pointer to the top of the structure

	// Ensure that utstr is a utstring.
	assert((*structPtr == SIGNATURE));

	// Return the length of utstr.
	uint32_t* lengthPtr = structPtr + 2;	// lengthPtr points to the length of utstr.
	return (*lengthPtr);
}

/**************************************************************************************************
                                           utrstrcat
This function takes a utstring s and concatenates it with the string suffix. The function updates
the length of s and then returns s.
**************************************************************************************************/
char* utstrcat(char* s, const char* suffix)
{
	uint32_t* structPtr = (uint32_t*)s - 3;	// Pointer to the top of the structure

	// Ensure that s is a utstring.
	assert((*(structPtr) == SIGNATURE));

	uint32_t capacity = *(structPtr + 1);	// Capacity of s

	uint32_t count = 0;						// Loop counter
	uint32_t offset = *(structPtr + 2);		// Index for traversing suffix, initialized to original length of string
	if (capacity == 0)
	{
		s[count] = '\0';
		return s;							// s has 0 capacity, so return from the function.
	}
	while (suffix[count] != '\0')			// While the end of the suffix has not been reached, repeat.
	{
		if ((capacity - *(structPtr + 2)) != 0)	// If capacity has not been reached, continue.
		{
			s[count + offset] = suffix[count];
			*(structPtr + 2) += 1;				// Update the length of s.
			count += 1;
		}
		else
		{
			break;								// End the loop once capacity has been reached.
		}
	}
	s[count + offset] = '\0';					// Append a null terminator to s.

	// Return s.
	return s;
}

/**************************************************************************************************
										utstrcpy
This function takes a utstring dst and overwrites the characters in dst with the characters from 
src.
**************************************************************************************************/
char* utstrcpy(char* dst, const char* src) 
{
	uint32_t* structPtr = (uint32_t*)dst - 3;	// Pointer to the top of the structure

	// Ensure that dst is a utstring.
	assert((*(structPtr) == SIGNATURE));

	// Store the capacity of dst.
	uint32_t dstCapacity = *(structPtr + 1);

	// Copy the characters in src to dst.
	uint32_t ind = 0;			// Index for traversing the arrays
	if (dstCapacity == 0)
	{
		dst[ind] = '\0';		// Append a null terminator to dst.
		return dst;				// dst has 0 capacity, so return from function.
	}
	while (src[ind] != '\0')	// While the end of src has not been reached, repeat.
	{ 
		if ((dstCapacity - *(structPtr + 2)) != 0)	// If capacity has not been reached, continue.
		{ 
			dst[ind] = src[ind];
			*(structPtr + 2) += 1;	// Continuously update the length.
			ind += 1;
		}
		else 
		{
			break;					// End the loop once capacity has been reached.
		}
	} 
	dst[ind] = '\0';	// Append a null terminator to dst.

	// Return dst.
	return dst;
}

/**************************************************************************************************
									utrstrfree
This function takes a utstring self and deallocates the storage for this string.
**************************************************************************************************/
void utstrfree(char* self) 
{
	// Calculate the address of the start of the chunk.
	uint32_t* chunkStart = (uint32_t*)self - 3;

	// Ensure that client really did pass a utstring.
	assert((*chunkStart == SIGNATURE));

	void* freePtr = chunkStart;		// Assign chunkStart to a void pointer type

	// Deallocate memory.
	free(freePtr);
}

/**************************************************************************************************
                                    utstrrealloc
This function takes a utstring s and:
	If new_capacity is equal or less than the current capacity of s, returns s.
	If new_capacity is greater than the current capacity of s, allocates a new struct with the
	new capacity and copies the characters of s into the new struct. Then, updates the metadata
	of the new struct and deallocates s, returning a pointer to the first  character of the 
	newly allocated storage.
**************************************************************************************************/
char* utstrrealloc(char* s, uint32_t new_capacity) 
{
	uint32_t* structPtr = (uint32_t*)s - 3;		// Pointer to the top of the structure

	// Ensure that dst is a utstring.
	assert((*(structPtr) == SIGNATURE));

	if (*(structPtr + 1) >= new_capacity)		// If s's capacity is greater than or equal to the new capacity, return
	{											// s.
		return s;
	}

	// Create a new struct with the new capacity. newStruct points to the new pointer.
	uint32_t* newStruct = (uint32_t*)malloc(sizeof(uint32_t) * 3 + new_capacity + 1);
	*newStruct = SIGNATURE;					// Signature
	*(newStruct + 1) = new_capacity;		// Update capacity.
	*(newStruct + 2) = *(structPtr + 2);	// Update length.
	char* newString = (char*)(newStruct + 3);	// newString points to the string in the new struct.
	uint32_t index = 0;							// Variable used to traverse the arrays
	// Copy the string in the original utstring to the new utstring.
	while (s[index] != '\0')
	{
		newString[index] = s[index];
		index += 1;
	}
	newString[index] = '\0';

	// Deallocate s.
	utstrfree(s);

	// Return pointer to string in new struct.
	return newString;
}




