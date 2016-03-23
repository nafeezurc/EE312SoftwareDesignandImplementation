/*
 * Project1.cpp
 * 
 * Name: Gopika Ajaykumar
 * EE312 Spring 2016
 * SpellCheck
 */

#include <stdio.h>  // provides declarations for printf and putchar
#include <stdint.h> // provides declarations for int32_t uint32_t and the other (new) standard C types

/* All of your code must be in this file. Please no #includes other than standard system headers (ie.., stdio.h, stdint.h)
 *
 * Many students find it helpful to declare global variables (often arrays). You are welcome to use
 * globals if you find them helfpul. Global variables are by no means necessary for this project.
 */

 /* You must write this function (spellCheck). Do not change the way the function is declared (i.e., it has
  * exactly two parameters, each parameter is a standard (mundane) C string (see SpellCheck.pdf).
  * You are expected to use reasonable programming style. I *insist* that you indent
  * reasonably and consistently in your code. I strongly encourage you to avoid big functions
  * So, plan on implementing spellCheck by writing two or three other "support functions" that
  * help make the actual spell checking easier for you.
  * There are no explicit restictions on using functions from the C standard library. However,
  * for this project you should avoid using functionality from the C++ standard libary. You will
  * almost certainly find it easiest to just write everything you need from scratch!
  */

// Function Prototypes
	void spellCheck(char[], char[]);
	uint8_t isLetter(char);
	void dictionaryCheck(char[], uint32_t);

// Global Arrays
char word[100];		// Global array to hold a word (Assumes no word is greater than 100 letters to avoid memory allocation)

/********************************************************
				 spellCheck Function
article[] is a pointer to an array of characters
containing an article that needs to be spellchecked.
The end of the article is marked with a 0. This
function prints every word in the article that cannot be
found in the dictionary. dictionary[] is a character
string ending in a 0. The end of each word in the
dictionary is marked with a '\n'.
********************************************************/
void spellCheck(char article[], char dictionary[]) {
	uint32_t index = 0;	// The index variable holds the element number currently being traversed in article[].

	while (article[index] != 0)	// While the end of the article has not been reached, do the following.
	{
		if (isLetter(article[index]))	// The current element of the array is a letter.
		{
			if (isLetter(article[index + 1]))	// The next element is also a letter; a word has been found!
			{
				uint32_t wordSize = 0;
				uint32_t wordIndex = 0;
				while (isLetter(article[index]))
				{
					word[wordIndex] = article[index];	// Store word in another array.
					wordIndex++;
					wordSize++;							// Find the size of the word.
					index++;
				}
				word[wordIndex] = 0;					// End the word with a null terminator.
				dictionaryCheck(dictionary, wordSize);	// Find the word in the dictionary.
			}
		}
		if (article[index] != 0)						// Find the next word, unless the end of the article has been reached.
		{
			index++;	// Go to the next element of the array.
		}
	}
}

/********************************************************
				 isLetter Function
This function checks if the character passed into it is
a letter (a-z or A-Z). If the character is a letter,
the function returns a 1. Otherwise, the function
returns a 0.
********************************************************/
uint8_t isLetter(char input) {
	if (((input >= 'a') && (input <= 'z')) || ((input >= 'A') && (input <= 'Z'))) // The input is a letter.
	{
		return 1;
	}
	return 0;	// The input is not a letter.
}

/********************************************************
			   dictionaryCheck Function
This function checks if a word is in the dictionary.
If the word is in the dictionary, the function
returns. Otherwise, the function prints the word and
then returns.
********************************************************/
void dictionaryCheck(char dictionary[], uint32_t size)
{
	uint32_t wordIndex = 0;	// Holds the index being traversed of the word array
	uint32_t dicIndex = 0;	// Holds the index being traversed of the dictionary array
	uint8_t matchFlag = 1;	// If matchFlag is set, a match has been found in the dictionary.
	while (dictionary[dicIndex] != 0)	// While the end of the dictionary has not been reached, continue.
	{
		matchFlag = 1;
		while (dictionary[dicIndex] == '\n')	// If the current character is a newline, go to the next character.
		{
			dicIndex++;
		}
		for (wordIndex = 0; wordIndex < size; wordIndex++)	// Compare the word with dictionary word.
		{
			// Capitalize both letters being compared.
			if ((word[wordIndex] >= 'a') && (word[wordIndex] <= 'z'))	// If the current character of the word is lowercase...
			{
				word[wordIndex] -= 32;	// Capitalize.
			}
			if ((dictionary[dicIndex] >= 'a') && (dictionary[dicIndex] <= 'z')) // If current dictionary character is lowercase...
			{
				dictionary[dicIndex] -= 32;	// Capitalize.
			}

			if (word[wordIndex] != dictionary[dicIndex])	// The two characters being compared are not equal.
			{
				matchFlag = 0;
				
				if (dictionary[dicIndex] == '\n')			// The dictionary word is shorter than the searched word.
				{
					break;									// Break out of the for loop.
				}
				
			}
			dicIndex++;
		} // End of for loop
			while (dictionary[dicIndex] != '\n' && dictionary[dicIndex] != 0)	// The two words are not the same size.
			{
				matchFlag = 0;
				dicIndex++;
			}
			if (matchFlag == 1)	// A match was found.
				return;			// The function returns if there was a match.
	}
	printf("%s\n", word);		// The whole dictionary was searched and a match was not found. Print the word.
}