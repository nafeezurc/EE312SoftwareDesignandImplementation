// ExpressionTree.h
// Gopika Ajaykumar
// Contains ExpressionTree class and its functions
// Last updated 04/21/2016

#pragma once
#ifndef _EXPRESSIONTREE_h
#define _EXPRESSIONTREE_h

// Included Files
#include "String.h"			// Include String class and its functions
#include <assert.h>			// Enables assertions
#include "Parse.h"			// Enables reading of Blip program

// Global Variables
extern BST<String> variables;		// variables is defined in Project9.cpp

struct ExpressionTree
{
private:
	struct Node
	{
		Node* left;			// Pointer to the left node
		Node* right;		// Pointer to the right node
		String string;		// If the node contains an operator or variable, then string will contain the symbol. 
							// Otherwise, it should hold an empty string (the null terminator).
		int num;			// If the node contains a number, then num will contain the number. 
							// Otherwise, it should hold -1.
		Node(void) { left = right = nullptr; num = -1; }										// Default Constructor
		Node(String str, int number) { left = right = nullptr; string = str;  num = number; }	// Constructor with 
	};																							// parameters

	Node* root;				// Pointer to the root of the tree

	/********************************************************************************************************************
													recursiveBuild
	recursiveBuild is a recursive function that builds a parse tree containing an expression. The function uses the
	Blip reading functions and variables provided by Dr. Chase.
	********************************************************************************************************************/
	static Node* recursiveBuild()
	{
		// Get the new number, variable, or operator in the expression.
		read_next_token();

		// Base Case: The current node is a leaf containing a variable or a number.
		if (next_token_type == NUMBER)							// The current token is a number.
		{
			Node *newNode = new Node("", token_number_value);
			return newNode;
		}
		if (next_token_type == NAME)							// The current token is a variable.
		{
			Node *newNode = new Node(String(next_token()), -1);
			return newNode;
		}

		// Inductive Step
		Node* root = new Node(String(next_token()), -1);		// The current token is an operator.

		// Get the left operand. 
		root->left = recursiveBuild();

		// If the operator is binary, get the right operand.
		if (root->string != "!" && root->string != "~")
		{
			root->right = recursiveBuild();
		}

		return root;
	}

	/********************************************************************************************************************
														evaluateExpression
	evaluateExpression is a recursive function that evaluates the expression stored in an ExpressionTree object.
	********************************************************************************************************************/
	static int evaluateExpression(Node* root)
	{
		// Base Case: The current node contains a number or variable, or the current node is the nullptr.
		if (root == nullptr)
		{
			return 0;
		}

		String str = root->string;

		if (str == "")							// This is a number.
		{
			return root->num;
		}

		if (variables.variableExists(str))		// This is a variable.
		{
			return (variables.search(str));
		}

		// Inductive Step
		int leftEval = evaluateExpression(root->left);
		int rightEval = evaluateExpression(root->right);

		// Operator Cases
		if (str == "+")
		{
			return (leftEval + rightEval);
		}
		else if (str == "-")
		{
			return (leftEval - rightEval);
		}
		else if (str == "*")
		{
			return (leftEval * rightEval);
		}
		else if (str == "/")
		{
			return (leftEval / rightEval);
		}
		else if (str == "%")
		{
			return (leftEval % rightEval);
		}
		else if (str == "&&")
		{
			return (leftEval && rightEval);
		}
		else if (str == "||")
		{
			return (leftEval || rightEval);
		}
		else if (str == "<")
		{
			return (leftEval < rightEval);
		}
		else if (str == ">")
		{
			return (leftEval > rightEval);
		}
		else if (str == "==")
		{
			return (leftEval == rightEval);
		}
		else if (str == "!=")
		{
			return (leftEval != rightEval);
		}
		else if (str == "<=")
		{
			return (leftEval <= rightEval);
		}
		else if (str == ">=")
		{
			return (leftEval >= rightEval);
		}
		else if (str == "!")
		{
			return (!leftEval);
		}
		else if (str == "~")
		{
			return (-1 * leftEval);
		}
	}

	/********************************************************************************************************************
														recursiveDelete
	recursiveDelete is a recursive function that deallocates the ExpressionTree's data.
	********************************************************************************************************************/
	static void recursiveDelete(Node* root) 
	{
		// Base Case
		if (root == nullptr) 
		{
			return;
		}

		// Inductive Step
		recursiveDelete(root->left);
		recursiveDelete(root->right);

		delete root;
	}

public:
	ExpressionTree(void) { root = nullptr; }				// Default Constructor
	~ExpressionTree(void) { recursiveDelete(root);  }		// Destructor

	void build(void)
	{
		root = recursiveBuild();
	}
	
	/********************************************************************************************************************
															evaluate
	evaluate is a public function that builds an ExpressionTree and returns the integer value of the evaluated 
	expression stored in the tree.
	********************************************************************************************************************/
	int evaluate()
	{
		root = recursiveBuild();
		return (evaluateExpression(root));
	}
};


#endif