// StatementTree.h
// Gopika Ajaykumar
// Contains StatementTree class and its functions
// Last updated 05/02/2016

#pragma once
#ifndef _STATEMENTTREE_h
#define _STATEMENTTREE_h

// Included Files
#include <stdio.h>
#include "Parse.h"				// Enables reading of Blip program
#include "ExpressionTree.h"		// Contains the ExpressionTree class and its functions
#include "Vector.h"				// Contains the Vector class and its functions
#include "BST.h"				// Contains the BST class and its functions

// Global Variables
BST<String> variables;			// BST that holds the variables; Also used in ExpressionTree.h

struct StatementTree
{
private:
	// Nested Class: Node
	struct Node 
	{
		String flag;					// flag is an attribute that indicates the command type.
		Vector<Node *> statements;		// statements is a vector of statement nodes
		Vector<Node *> elseStatements;	// elseStatements is a vector of statement nodes to be executed for false condition
		ExpressionTree* exp;			// exp is a pointer to an ExpressionTree
		String text;					// text is a String to be outputted
		String variable;				// variable is a String representing a variable name

		// Node's Member Functions
		// Default Constructor
		Node(const String& type, Vector<Node*>& stateVec, ExpressionTree* expression, const String& txt, const String& varName)
		{
			flag = "";
			exp = nullptr;
			text = "";
			variable = "";
		}

		// text constructor
		Node(const String& type, const String& txt)											
		{
			flag = type;
			text = txt;
			exp = nullptr;
		}

		// output constructor
		Node(const String& type, ExpressionTree* expression)							
		{
			flag = type;
			exp = expression;
		}

		// var/set constructor
		Node(const String& type, const String& varName, ExpressionTree* expression)			
		{
			flag = type;
			variable = varName;
			exp = expression;
		}

		// do constructor
		Node(const String& type, ExpressionTree* expression, Vector<Node*>& stateVec)	
		{
			flag = type;
			exp = expression;
			statements = stateVec;
		}

		// if/else constructor
		Node(const String& type, ExpressionTree* expression, Vector<Node*>& stateVec, Vector<Node*>& elseStateVec)
		{
			flag = type;
			exp = expression;
			statements = stateVec;
			elseStatements = elseStateVec;
		}
	};	// end nested Node class

	Node* root;							// root of the StatementTree (unique node with no parents)

	/*******************************************************************************************************************
														ifBuild
	ifBuild creates a StatementTree node representing an if command. This function is called by the recursiveBuild
	function (see below).
	*******************************************************************************************************************/
	static Node* ifBuild(void)
	{
		String keyword("if");

		ExpressionTree* expression = new ExpressionTree;
		expression->build();

		Vector<Node*> statementTable;
		Vector<Node*> elseStatementTable;

		bool isElse = true;							// This flag is set to false if there is no else condition.
		read_next_token();
		while (String(next_token()) != "else")
		{
			if (String(next_token()) == "fi")
			{
				isElse = false;
				break;
			}
			Node* newNode = recursiveBuild();		 // Recursive call (calls recursiveBuild again)
			statementTable.push_back(newNode);
			read_next_token();
		}

		if (isElse)
		{
			read_next_token();
			while (String(next_token()) != "fi")
			{
				Node* newNode = recursiveBuild();	// Recursive call (calls recursiveBuild again)
				elseStatementTable.push_back(newNode);
				read_next_token();
			}
		}

		Node* root = new Node(keyword, expression, statementTable, elseStatementTable);
		return root;
	}

	/*******************************************************************************************************************
	                                                    doBuild
	doBuild creates a StatementTree node representing a do command. This function is called by the recursiveBuild
	function (see below).
	*******************************************************************************************************************/
	static Node* doBuild(void)
	{
		String keyword("do");

		ExpressionTree* expression = new ExpressionTree;
		expression->build();

		Vector<Node*> statementTable;		
		read_next_token();
		while (String(next_token()) != "od")
		{	
			Node* newNode = recursiveBuild();	   // Recursive call (calls recursiveBuild again)
			statementTable.push_back(newNode);
			read_next_token();
		}		

		Node* root = new Node(keyword, expression, statementTable);
		return root;
	}

	/*******************************************************************************************************************
	                                                    recursiveBuild
	recursiveBuild is a recursive function that builds a parse tree containing a statement. The function uses the
	Blip reading functions and variables provided by Dr. Chase.
	*******************************************************************************************************************/
	static Node* recursiveBuild(void)
	{
		// Get the command.
		String keyword(next_token());								// String object containing token

		// Base Cases
		if (keyword == "text")
		{
			read_next_token();
			String str(next_token());
			Node* root = new Node(keyword, str);					// Use the text constructor to create a new node.
			return root;
		}
		else if (keyword == "output")
		{
			ExpressionTree* expression = new ExpressionTree;
			expression->build();
			Node* root = new Node(keyword, expression);				// Use the output constructor to create a new node.
			return root;
		}
		else if (keyword == "var" || keyword == "set")
		{
			read_next_token();
			String varName(next_token());
			ExpressionTree* expression = new ExpressionTree;
			expression->build();
			Node* root = new Node(keyword, varName, expression);	// Use the var/set constructor to create a new node.
			return root;
		}
		else if (keyword == "do")									// Inductive steps
		{
			Node* root = doBuild();
			return root;
		}
		else if (keyword == "if")
		{
			Node* root = ifBuild();
			return root;
		}
		else if (keyword == "//")									// Comment: No statements so return nullptr
		{
			skip_line();
			Node* root = nullptr;
			return root;
		}		
	}

	/********************************************************************************************************************
	                                             evaluateStatement
	evaluateStatement is a recursive function that evaluates the statement stored in an StatementTree object.
	********************************************************************************************************************/
	static void evaluateStatement(Node* root)
	{
		if (!root)													// nullptr indicates a comment.
		{
			return;
		}

		// Base Cases
		String type = root->flag;

		if (type == "text")
		{
			printf("%s", (root->text).c_str());
		}
		else if (type == "output")
		{
			printf("%d", (root->exp)->evaluate());
		}
		else if (type == "var")
		{
			// Check if the variable already exists.
			if (!variables.variableExists(root->variable))			// The variable was not found.
			{
				variables.insert(root->variable, (root->exp)->evaluate());
			}
			else													// The variable was found.
			{
				// Print an error message.
				printf("variable %s incorrectly re-initialized\n", (root->variable).c_str());
				variables.changeValue(root->variable, (root->exp)->evaluate());
			}
		}
		else if (type == "set")
		{
			// Check if the variable already exists.
			if (!variables.variableExists(root->variable))			// The variable was not found.
			{
				// Print an error message.
				printf("variable %s not declared\n", (root->variable).c_str());
				variables.insert(root->variable, (root->exp)->evaluate());
			}
			else													// The variable was found.
			{
				variables.changeValue(root->variable, (root->exp)->evaluate());
			}
		}
		else if (type == "do")
		{
			while ((root->exp)->evaluate())
			{
				for (uint32_t i = 0; i < (root->statements).size(); i += 1)
				{
					evaluateStatement(root->statements[i]);		// Recursive call for each of the statements in the vector
				}
			}
		}
		else if (type == "if")
		{
			if ((root->exp)->evaluate())
			{
				for (uint32_t i = 0; i < (root->statements).size(); i += 1)
				{
					evaluateStatement(root->statements[i]);
				}
			}
			else
			{
				for (uint32_t i = 0; i < (root->elseStatements).size(); i += 1)
				{
					evaluateStatement(root->elseStatements[i]);
				}
			}
		}
	}
public:
	StatementTree(void) { root = nullptr; }										// Default Constructor
	~StatementTree(void) { if (root) { delete root->exp; delete root; } }		// Destructor

	/*******************************************************************************************************************
														build
		build is a public function that builds a StatementTree by calling the recursiveBuild function.
	*******************************************************************************************************************/
	void build(void)
	{
		root = recursiveBuild();
	}

	/*******************************************************************************************************************
		                                             evaluate
     evaluate is a public function that evaluates the calling StatementTree.
	*******************************************************************************************************************/
	void evaluate(void)
	{
		evaluateStatement(root);
	}
}; 

#endif
