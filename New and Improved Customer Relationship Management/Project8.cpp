// Gopika Ajaykumar
// Project 8: CRM in C++

// Included Files
#include <stdio.h>
#include "String.h"
#include "Customer.h"
#include "CustomerDB.h"

// Function Prototypes
void readString(String&);
void readNum(int&);

CustomerDB database;	// Database for Ellie's Baby Emporium

// Global Variables
int num_bottles = 0;	// Holds the total number of bottles in inventory
int num_rattles = 0;	// Holds the total number of rattles in inventory
int num_diapers = 0;	// Holds the total number of diapers in inventory


/**********************************************************************************************************************
                                                   reset
This function clears the inventory and resets the customer database to empty. This function was given in the starter
code.
**********************************************************************************************************************/
void reset(void) 
{
	database.clear();
	num_bottles = 0;
	num_rattles = 0;
	num_diapers = 0;
}

 /**********************************************************************************************************************
                                           selectInventItem with String parameter
This function allows you to obtain a pointer to the inventory record using the item type name. word must be "Bottles", 
"Diapers" or "Rattles". This function was given in the starter code.
 **********************************************************************************************************************/
int* selectInventItem(String word) 
{
	if (word == "Bottles") 
	{
		return &num_bottles;
	} 
	else if (word == "Diapers") 
	{
		return &num_diapers;
	} 
	else if (word == "Rattles") 
	{
		return &num_rattles;
	}
	
	/* NOT REACHED */
	return 0;
}

 /**********************************************************************************************************************
                                    selectInventItem with String and Customer parameters
This overload of selectInventItem is similar to selectInventItem above. However, this overload takes a Customer as a 
second argument and selects the data member of that Customer that matches "word". The function returns a pointer to one 
of the three data members from the specified customer. This function was given in the starter code.
 **********************************************************************************************************************/
int* selectInventItem(String word, Customer& cust) 
{
	if (word == "Bottles") 
	{
		return &cust.bottles;
	} 
	else if (word == "Diapers") 
	{
		return &cust.diapers;
	} 
	else if (word == "Rattles") 
	{
		return &cust.rattles;
	}
	
	/* NOT REACHED */
	return 0;
}

 /**********************************************************************************************************************
                                                    findMax
This function searches through the CustomerDB "database" and returns the Customer who has purchased the most items of 
the specified type. type must be one of "Bottles", "Rattles" or "Diapers". Note: if two or more Customers are tied for 
having purchased the most of that item type, then findMax returns the first Customer in the CustomerDB who has purchased 
that maximal quantity. Note: in the special case (invalid case) where there are zero Customers in the CustomerDB, 
findMax returns a null pointer (0).
 **********************************************************************************************************************/
Customer* findMax(String type) 
{
	Customer* result = 0;
	int max = 0;
	for (int k = 0; k < database.size(); k += 1) 
	{
		Customer& cust = database[k];
		int *p = selectInventItem(type, cust); 
		if (*p > max) 
		{
			max = *p;
			result = &cust;
		}
	}
	
	return result;
}

/**********************************************************************************************************************
                                                processPurchase
processPurchase reads the customer's name, type of item purchased, and the number of items purchased and updates the
customer record and store inventory to reflect the purchase made. If the customer is a new visitor, then a new
customer record is created for him or her.
**********************************************************************************************************************/
void processPurchase() 
{
	String customerName;		// Holds the customer's name
	String itemType;			// Holds the item purchased
	int numItems;				// Holds the number of items purchased

	// Read from the file.
	readString(customerName);
	readString(itemType);
	readNum(numItems);

	int* inventoryTotal = selectInventItem(itemType);	// Number of the purchased item in inventory

	if (inventoryTotal == nullptr)						// ERROR: Invalid item
	{
		return;
	}

	// Account for invalid purchases.
	if (numItems <= 0)
	{
		return;
	}

	if (numItems > *inventoryTotal)
	{
		printf("Sorry %s, we only have %d %s\n", customerName.c_str(), *inventoryTotal, itemType.c_str());
	}
	else
	{
		*selectInventItem(itemType, database[customerName]) += numItems;
		*inventoryTotal -= numItems;
	}
}

/**********************************************************************************************************************
                                                processSummarize
processSummarize prints a summary of the current inventory, number of customers, and customers with the most purchases
of each item sold at Ellie's Baby Emporium.
**********************************************************************************************************************/
void processSummarize()
{
	String Bottles("Bottles");
	String Diapers("Diapers");
	String Rattles("Rattles");

	// Print the inventory.
	printf("There are %d Bottles, %d Diapers and %d Rattles in inventory\n", num_bottles, num_diapers, num_rattles);

	// Print the number of customers.
	printf("we have had a total of %d different customers\n", database.size());

	// Report "best" customers.
	Customer* maxBottleBuyer = findMax(Bottles);
	Customer* maxDiaperBuyer = findMax(Diapers);
	Customer* maxRattleBuyer = findMax(Rattles);

	// Print the results of the search.
	if (maxBottleBuyer)		// The max bottle purchase is greater than 0.
	{
		printf("%s has purchased the most Bottles (%d)\n", (maxBottleBuyer->name).c_str(), maxBottleBuyer->bottles);
	}
	else					// No bottles were purchased.
	{
		printf("no one has purchased any Bottles\n");
	}
	if (maxDiaperBuyer)		// The max diaper purchase is greater than 0.
	{
		printf("%s has purchased the most Diapers (%d)\n", (maxDiaperBuyer->name).c_str(), maxDiaperBuyer->diapers);
	}
	else					// No diapers were purchased.
	{
		printf("no one has purchased any Diapers\n");
	}
	if (maxRattleBuyer)		// The max rattle purchase is greater than 0.
	{
		printf("%s has purchased the most Rattles (%d)\n", (maxRattleBuyer->name).c_str(), maxRattleBuyer->rattles);
	}
	else					// No rattles were purchased.
	{
		printf("no one has purchased any Rattles\n");
	}
}




/**********************************************************************************************************************
                                       processInventory
processInventory reads the item type and quantity from the input file and updates the store’s inventory of the
indicated item type.
**********************************************************************************************************************/
void processInventory() 
{
	String itemType;							// String storing the item type
	int itemsReceived;							// Number of items received from the shipment

	// Read the type of the item.
	readString(itemType);

	// Read the number of items received.
	readNum(itemsReceived);

	// Invalid: Items received was negative or 0.
	if (itemsReceived <= 0)
	{
		return;
	}

	// Find out what item type was received, and update the store's inventory accordingly.
	int* currentInventory = selectInventItem(itemType);
	*currentInventory += itemsReceived;
}