// Project 9: Blip Interpreter A
// Main File
// Gopika Ajaykumar
// Last updated 04/22/2016

// Included Files
#include <stdio.h>			
#include "Parse.h"			// Contains function declarations and global variables for reading input from a Blip program
#include "String.h"			// Contains String class and its functions
#include "BST.h"			// Contains BST class and its functions
#include "ExpressionTree.h"	// Contains ExpressionTree class and its functions

// Function Definitions
void run(void);					// Runs a Blip program
int analyzeExpression();		// Analyzes an expression
void createVariable(String);	// Creates a variable in Blip storage
void changeVariable(String);	// Alters a variable in Blip storage

// Global Variables
BST<String> variables;			// BST that holds the variables; Also used in ExpressionTree.h

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
	// Get the next token.
	read_next_token();					// next_token_type and token_number_value (if token type is NUMBER) are set.

	while (next_token_type != END)		// While the end of the file has not been reached, keep reading.
	{
		String keyword(next_token());	// String containing the current keyword

		if (keyword == "text")			// Case 1: The keyword is 'text'.
		{
			read_next_token();			// Go to the next token. This should be a NAME.
			printf("%s", next_token());
		}	// end text case
		else if (keyword == "output")	// Case 2: The keyword is 'output'.
		{
			printf("%d", analyzeExpression());
		}	// end output case		
		else if (keyword == "var")		// Case 3: They keyword is 'var'.
		{
			read_next_token();				// Go to the operand for var.
			String varName(next_token());	// varName contains the variable to be initialized.

			// Check if the variable already exists.
			if (!variables.variableExists(varName))		// The variable was not found.
			{
				createVariable(varName);
			}
			else										// The variable was found.
			{
				// Print an error message.
				printf("variable %s incorrectly re-initialized\n", varName.c_str());
				changeVariable(varName);
			}	
		}	// end var case
		else if (keyword == "set")			// Case 4: The keyword is 'set'.
		{
			read_next_token();				// Go to the operand for set.
			String varName(next_token());	// varName contains the variable to be set.
													
			// Check if the variable already exists.
			if (!variables.variableExists(varName))			// The variable was not found.
			{
				// Print an error message.
				printf("variable %s not declared\n", varName.c_str());
				createVariable(varName);
			}
			else											// The variable was found.
			{
				changeVariable(varName);
			}
		}
		else if (keyword == "//")
		{
			skip_line();
		}

		// Get the next token.
		read_next_token();				// next_token_type and token_number_value (if token type is NUMBER) are set.
	}	// end file reading while
}	// end run

/************************************************************************************************************************
                                                   createVariable
createVariable creates a variable in Blip memory with the value specified.
************************************************************************************************************************/
void createVariable(String name)
{
	// Create the variable in Blip internal memory with the specified value.
	variables.insert(name, analyzeExpression());
}

/************************************************************************************************************************
                                                   changeVariable
changeVariable changes a variable in Blip memory to the value specified.
************************************************************************************************************************/
void changeVariable(String name)
{
	variables.changeValue(name, analyzeExpression());
}

/************************************************************************************************************************
													analyzeExpression
analyzeExpression inserts an expression into a parse tree, cmd. The function then calls cmd's evaluate function, which
returns the integer value equivalent to the expression. The function returns this integer value.
************************************************************************************************************************/
int analyzeExpression()
{
	ExpressionTree cmd;
	return cmd.evaluate();
}

