// Project 10 Blip Interpreter B
// Main File
// Gopika Ajaykumar
// Last updated 04/30/2016

// Included Files
#include "Parse.h"				// Contains function declarations and global variables for reading input from a Blip program
#include "String.h"			    // Contains the String class and its functions
#include "StatementTree.h"		// Contains StatementTree class and its functions

// Function Definitions
void run(void);					// Runs a Blip program

int main(void)
{
	run();
	return 0;
}

/************************************************************************************************************************
                                                   run
run runs a Blip program and analyzes its statements, performing actions such as console output, variable creation, and
variable manipulation according to the Blip commands in the program.
************************************************************************************************************************/
void run(void)
{
	read_next_token();
	while (next_token_type != END)
	{
		StatementTree statement;
		statement.build();
		statement.evaluate();	
		read_next_token();
	}
}