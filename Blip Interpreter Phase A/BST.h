// BST.h
// Class provided by Dr. Chase with edits by Gopika Ajaykumar (edits are indicated by comments)
// Contains BST class and its functions, edited to work specifically with variables in the Blip Language
// Last updated 04/21/2016 

#pragma once
#ifndef _BST_h
#define _BST_h

// Included Files
#include "String.h"			// Includes String class and its functions
#include <assert.h>			// Enables assertions

template <typename T>		// This code is a template. For this solution, we use the String type.

struct BST {
private:
	struct Node {
		T value;			// Value will be the variable's name
		int data;			// Edit: Added a data attribute that will include the integer value stored in the variable
		Node* left;
		Node* right;
		Node* parent;

		Node(T value, int data) {				// Edit: Constructor now takes int data to be stored in variable
			this->value = value;
			this->data = data;
			left = right = parent = nullptr;
		}

		~Node(void) {
			delete left;
			delete right;
		}
	};

private:

	static Node* recursiveCopy(Node* root) {	// Edit: int data is now copied too.
		if (root == nullptr) { return 0; }

		Node* result = new Node(root->value, root->data);

		Node* left = recursiveCopy(root->left);
		Node* right = recursiveCopy(root->right);

		result->left = left;
		result->right = right;
		if (left != nullptr) { left->parent = result; }
		if (right != nullptr) { right->parent = result; }

		return result;
	}

	static Node* successor(Node* p) {
		if (p->right != 0) { // p has a right subtree
							 /* successor to p will be smallest node in right subtree */
			Node* next = p->right;
			while (next->left != 0) {
				next = next->left;
			}
			return next;
		} else { // p does not have a right subtree
				 /* successor to p will be closest ancestor where we go up a left branch */
			Node* parent = p->parent;
			while (parent != 0 && parent->right == p) {
				p = parent;
				parent = p->parent;
			}
			return parent;
		}
	}

	void remove(Node* p) {
		if (p == 0) { // WTF?
			return;
		}

		if (p->right == nullptr) { // easy case! 
			Node* child = p->left;
			Node* parent = p->parent;
			if (parent == nullptr) {
				if (child != nullptr) {
					child->parent = nullptr;
				} 
				root = child;
				p->left = p->right = nullptr;
				delete p;
				return;
			}
			if (p == parent->left) {
				parent->left = child;
			} else {
				parent->right = child;
			}
			if (child != nullptr) {
				child->parent = parent;
			}
			delete p;
		} else { // nasty case! 
			Node* victim = successor(p);
			p->value = victim->value;
			Node* parent = victim->parent;
			Node* child = victim->right;
			if (parent->left == victim) {
				parent->left = child;
			} else {
				parent->right = child;
			}
			if (child != nullptr) {
				child->parent = parent;
			}
			victim->right = victim->left = nullptr;
			delete victim;
		}
	}

	static Node* find(Node* root, T value) {
		while (root != 0 && root->value != value) {
			if (value < root->value) {
				root = root->left;
			} else {
				root = root->right;
			}
		}
		return root;
	}

	Node* root;

public:

	BST(void) { root = nullptr; }
	~BST(void) {
		delete root;
	}

	BST(const BST& that) {
		root = recursiveCopy(that.root);
	}

	BST& operator=(const BST& that) {
		if (this != &that) {
			delete root;
			root = recursiveCopy(that.root);
		}
		return *this;
	}

	void insert(T value, int data) {			// Edit: The function now inserts a node with specified int data.
		Node* child = new Node(value, data);

		if (root == nullptr) {
			root = child;
			return;
		}

		Node* parent = nullptr;
		Node* p = root;
		while (p != nullptr) {
			parent = p;
			if (value < p->value) {
				p = p->left;
			} else {
				p = p->right;
			}
		}

		if (value < parent->value) {
			assert(parent->left == nullptr);
			parent->left = child;
		} else {
			assert(parent->right == nullptr);
			parent->right = child;
		}
		child->parent = parent;
	}

	void remove(T value) {
		Node* p = find(root, value);
		remove(p);
	}

	void print() {
		if (root == nullptr) { return; } // special case for empty tree


		Node* p;

		/* set p to smallest */
		p = root;
		while (p->left != nullptr) {
			p = p->left;
		}

		const char* prefix = "";
		while (p != nullptr) {
			printf("%s", prefix);
			printf("%s: %d", (p->value).c_str(), p->data);	// Edit: Now the int data is printed too.
			prefix = ", ";
			p = successor(p);
		}
		printf("\n");

	}

	// Edit: changeValue is a function created by me. The function takes in a variable name and the new int data that 
	// should be associated with it. Using Dr. Chase's find function, the function finds the node associated with the 
	// varName and changes its value to newValue.
	void changeValue(T varName, int newValue)
	{
		Node* changedNode = find(root, varName);
		assert(changedNode != nullptr);				// Error: Trying to change the value of a variable that does not exist
		changedNode->data = newValue;
	}

	// Edit: search is a function created by me. The function takes in a variable name and finds the node associated with
	// the name, returning the integer data stored in the node. 
	int search(T varName)
	{
		Node *p = find(root, varName);
		assert(p->value == varName);				// Error: The variable does not exist.
		return p->data;
	}

	// Edit: variableExists is a function created by me. The function takes in a variable name and returns true if the
	// variable exists and false otherwise. The function uses Dr. Chase's find function.
	bool variableExists(T varName)
	{
		Node *p = find(root, varName);
		if (p == nullptr)
		{
			return false;
		}
		return true;
	}
};


#endif /* _BST_h */