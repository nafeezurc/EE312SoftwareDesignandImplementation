// Gopika Ajaykumar
// Member Functions of CustomerDB and Customer

// Included Files
#include <iostream>
#include <cassert>
#include "String.h"
#include "CustomerDB.h"

using namespace std;

const int default_capacity = 1;		// Default capacity is 1.

/*************************************************************************************************************************************
                                                   Customer Constructor
This constructor creates a Customer with bottles, rattles, and diapers initialized to 0. The argument name is assigned to the 
Customer's name attribute. This function was given in the starter code.
*************************************************************************************************************************************/
Customer::Customer(String name) 
{
	this->bottles = 0;
	this->rattles = 0;
	this->diapers = 0;
	this->name = name;
}

/*************************************************************************************************************************************
                                                  CustomerDB Constructor
This constructor creates a CustomerDB with default capacity and a length of 0. This function was given in the starter code.
*************************************************************************************************************************************/
CustomerDB::CustomerDB(void) 
{ 
	this->capacity = default_capacity;
	this->data = new Customer[this->capacity];
	this->length = 0;
}

/*************************************************************************************************************************************
                                                        size
This function returns the size of the CustomerDB that called it. This function was given in the starter code.
*************************************************************************************************************************************/
int CustomerDB::size(void) 
{
	return this->length;
}

/*************************************************************************************************************************************
                                                    CustomerDB Destructor
This function deallocates the database associated with a CustomerDB. This function was given in the starter code.
*************************************************************************************************************************************/
CustomerDB::~CustomerDB(void) 
{ 
	delete[] this->data;
}

/*************************************************************************************************************************************
                                                       clear
This function deletes the database associated with the CustomerDB that called it and creates a new database with default capacity
and a length of 0. This function was given in the starter code.
*************************************************************************************************************************************/
void CustomerDB::clear(void) 
{
	delete[] this->data;
	this->capacity = default_capacity;
	this->data = new Customer[this->capacity];
	this->length = 0;
}

/*************************************************************************************************************************************
                                                operator[] with int parameter
This function returns the Customer in the database stored in index k. This function was given in the starter code.
*************************************************************************************************************************************/
Customer& CustomerDB::operator[](int k) 
{ 
	assert(k >= 0 && k < this->length);
	return this->data[k];
}

/*************************************************************************************************************************************
                                                 operator[] with String parameter
If a Customer in the CustomerDB has a name that matches the argument, name, then the function returns that Customer by reference. If
there is no Customer in the CustomerDB with that name, then the function adds a new Customer to the database and then returns a 
reference to the newly added Customer. This function uses amortized doubling.
*************************************************************************************************************************************/
Customer& CustomerDB::operator[](String name) 
{ 
	// Check if the argument is the name of an existing customer.
	bool customerFound = isMember(name);

	if (customerFound)					// If the customer is already in the database, continue.
	{
		int i = 0;
		for (i = 0; i < length; i += 1)
		{
			if (data[i].name == name)
			{
				return data[i];			// Return the customer.
			}
		}
	}

	// Add the new customer to the record.
	if (capacity == length)								// The database is currently full.
	{
		Customer* newDB = new Customer[capacity * 2];	// Create a new expanded database. Use amortized doubling.
		int i = 0;
		for (i = 0; i < length; i += 1)					// Copy contents to the expanded database.
		{
			newDB[i] = data[i];
		}
		delete[] data;									// Deallocate the old database.
		data = newDB;									// data points to the new expanded database.
		capacity *= 2;									// Amortized Doubling
	}
	data[length] = Customer(name);						// Add the new customer to the database.
	length += 1;										// Increment the length of the databse.
	return data[length - 1];							// Return the new customer.
}

/*************************************************************************************************************************************
                                                    isMember
This function searches through the current set of Customers and returns true if it finds a Customer with the matching name (and 
returns false otherwise).
*************************************************************************************************************************************/
bool CustomerDB::isMember(String name) 
{ 
	int i = 0;
	for (i = 0; i < length; i += 1)
	{
		if (data[i].name == name)		// The customer was found.
		{
			return true;
		}
	}
	return false;						// The customer was not found.
}


