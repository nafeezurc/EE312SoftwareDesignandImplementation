/*
* EE312 Simple CRM Project
*
* Gopika Ajaykumar
* EE312, Spring 2016
*/

// Included Files
#include <stdio.h>
#include <assert.h>
#include "MyString.h"
#include "Invent.h"
#include <stdint.h>

// Maximum customers that will visit the store
#define MAX_CUSTOMERS 1000

// Store Structure
struct Store							
{
	uint64_t bottleInventory;			// Stores the inventory of bottles
	uint64_t diaperInventory;			// Stores the inventory of diapers
	uint64_t rattleInventory;			// Stores the inventory of rattles
	uint64_t numCustomers;				// Stores the number of customers
	Customer customers[MAX_CUSTOMERS];	// Customer Database
};

// Function Prototypes
void maximumPurchases(Customer*, uint64_t);		// Finds the customer with the max purchases of each item, if any

// Initialize a Store.
Store ellies = { 0, 0, 0, 0 };

/**********************************************************************************************************************
                                               reset
reset clears the inventory of Ellie's Baby Emporium and resets its customer database to empty.
**********************************************************************************************************************/
void reset(void) 
{
	// Clear the inventory.
	ellies.bottleInventory = 0;
	ellies.diaperInventory = 0;
	ellies.rattleInventory = 0;

	// Reset the customer database to empty.
	for (uint64_t count = 0; count < ellies.numCustomers; count += 1)
	{
		StringDestroy(&ellies.customers[count].name);
		ellies.customers[count].bottles = 0;
		ellies.customers[count].diapers = 0;
		ellies.customers[count].rattles = 0;
	}

	ellies.numCustomers = 0;
}

/**********************************************************************************************************************
                                           processSummarize
processSummarize prints a summary of the current inventory, number of customers, and customers with the most purchases
of each item sold at Ellie's Baby Emporium.
**********************************************************************************************************************/
void processSummarize(void)
{
	// Print the inventory.
	printf("There are %llu Bottles, %llu Diapers and %llu Rattles in inventory\n", ellies.bottleInventory,
			ellies.diaperInventory, ellies.rattleInventory);

	// Print the number of customers.
	printf("we have had a total of %llu different customers\n", ellies.numCustomers);

	// Report "best" customers.
	maximumPurchases(ellies.customers, ellies.numCustomers);
}

/**********************************************************************************************************************
                                                processPurchase
processPurchase reads the customer's name, type of item purchased, and the number of items purchased and updates the
customer record and store inventory to reflect the purchase made. If the customer is a new visitor, then a new 
customer record is created for him or her.
**********************************************************************************************************************/
void processPurchase(void)
{
	// Local Variables
	String Bottles = StringCreate("Bottles");	// String that holds the word Bottles
	String Diapers = StringCreate("Diapers");	// String that holds the word Diapers
	String Rattles = StringCreate("Rattles");	// String that holds the word Rattles
	String customerName;						// String that holds the customer name
	String type;								// String that holds the item type
	int itemsPurchased;							// Variable that holds the number of items purchased by the customer
	bool foundCustomer = false;					// If the customer is found, this flag is set to true.
	bool invalidPurchase = false;				// If the purchase is invalid, this flag is set to true.

	// Read the customer's name.
	readString(&customerName);

	// Read the item type.
	readString(&type);

	// Read the number of items purchased.
	readNum(&itemsPurchased);

	// Account for invalid purchases.
	if (itemsPurchased <= 0)							// The purchase is 0 or negative.
	{
		invalidPurchase = true;
		itemsPurchased = 0;								// Make negative purchase 0
	}

	if (StringIsEqualTo(&type, &Bottles))				// Bottles were purchased.
	{
		if (itemsPurchased > ellies.bottleInventory)	// There are not enough bottles in inventory.
		{
			printf("Sorry ");
			StringPrint(&customerName);
			printf(", we only have %llu ", ellies.bottleInventory);
			StringPrint(&type);
			printf("\n)");
			invalidPurchase = true;
		}
	}
	else if (StringIsEqualTo(&type, &Diapers))			// Diapers were purchased.
	{
		if (itemsPurchased > ellies.diaperInventory)	// There are not enough diapers in inventory.
		{
			printf("Sorry ");
			StringPrint(&customerName);
			printf(", we only have %llu ", ellies.diaperInventory);
			StringPrint(&type);
			printf("\n");
			invalidPurchase = true;
		}
	}
	else if (StringIsEqualTo(&type, &Rattles))			// Rattles were purchaed.	
	{
		if (itemsPurchased > ellies.rattleInventory)	// There are not enough rattles in inventory.
		{
			printf("Sorry ");
			StringPrint(&customerName);
			printf(", we only have %llu ", ellies.rattleInventory);
			StringPrint(&type);
			printf("\n");
			invalidPurchase = true;
		}
	}
	else												// Item not sold in store
	{
		invalidPurchase = true;
	}

	if (invalidPurchase == false)	// If the purchase was valid, continue. Otherwise, skip and destroy the created
	{							    // strings.
		// Look up the customer in the customer database.
		uint64_t index = 0;			// Index to traverse the customers array
		for (index = 0; index < ellies.numCustomers; index += 1)
		{
			if (StringIsEqualTo(&(ellies.customers[index].name), &customerName))	// Customer found in records
			{
				foundCustomer = true;
				break;	// Break out of the for loop.
			}
		}

		/* Once the for loop ends, index will contain either the index of the customer, or if this is a new customer,
		numCustomers.*/
		if (index != MAX_CUSTOMERS)		// The customer array is not full.
		{
			if (foundCustomer == 0)		// This is a new customer. Create a customer record for him or her.
			{
				ellies.customers[index].name = StringDup(&customerName);
				ellies.numCustomers += 1;	// Ellie's gained a new customer.
			}

			// Update the store inventory and customer record.
			if (StringIsEqualTo(&type, &Bottles))					// Bottles were purchased.
			{
				ellies.bottleInventory -= itemsPurchased;			// Update the store inventory.
				ellies.customers[index].bottles += itemsPurchased;	// Update the customer record.
			}
			else if (StringIsEqualTo(&type, &Diapers))				// Diapers were purchased.
			{
				ellies.diaperInventory -= itemsPurchased;			// Update the store inventory.
				ellies.customers[index].diapers += itemsPurchased;	// Update the customer record.
			}
			else if (StringIsEqualTo(&type, &Rattles))				// Rattles were purchaed.	
			{
				ellies.rattleInventory -= itemsPurchased;			// Update the store inventory.
				ellies.customers[index].rattles += itemsPurchased;	// Update the customer record.
			}
		}
	}

	// Destroy the created strings.
	StringDestroy(&customerName);
	StringDestroy(&type);
	StringDestroy(&Bottles);
	StringDestroy(&Diapers);
	StringDestroy(&Rattles);
}

/**********************************************************************************************************************
                                           processInventory
processInventory reads the item type and quantity from the input file and updates the store’s inventory of the 
indicated item type.
**********************************************************************************************************************/
void processInventory(void)
{
	// Local Variables
	String Bottles = StringCreate("Bottles");	// String that holds the word Bottles
	String Diapers = StringCreate("Diapers");	// String that holds the word Diapers
	String Rattles = StringCreate("Rattles");	// String that holds the word Rattles
	String itemType;							// String storing the item type
	int itemsReceived;							// Number of items received from the shipment

	// Read the type of the item.
	readString(&itemType);

	// Read the number of items received.
	readNum(&itemsReceived);

	if (itemsReceived < 0)						// Invalid: Items received was negative.
		itemsReceived = 0;

	// Find out what item type was received, and update the store's inventory accordingly.
	if (StringIsEqualTo(&itemType, &Bottles))		// Bottles were received.
	{
		ellies.bottleInventory += itemsReceived;	// Update the store inventory.
	}
	else if (StringIsEqualTo(&itemType, &Diapers))	// Diapers were received.
	{
		ellies.diaperInventory += itemsReceived;	// Update the store inventory.
	}
	else if (StringIsEqualTo(&itemType, &Rattles))	// Rattles were received.	
	{
		ellies.rattleInventory += itemsReceived;	// Update the store inventory.
	}

	// Destroy the created strings.
	StringDestroy(&itemType);
	StringDestroy(&Bottles);
	StringDestroy(&Diapers);
	StringDestroy(&Rattles);
}

/**********************************************************************************************************************
                                           maximumPurchases
maximumPurchases outputs to the console the customer who made the most purchases for each item in the Ellie's Baby
Emporium.
**********************************************************************************************************************/
void maximumPurchases(Customer* allCustomers, uint64_t numberOfCustomers)
{
	uint64_t index = 0;								// Variable used to traverse the array of customers

	uint16_t maxDiaperIndex = 0;					// Variable that will store the index of the customer who purchased 
													// the most diapers
	uint64_t maxDiapers = 0;						// Variable that will store the largest number of diaper purchases
					
	uint16_t maxBottleIndex = 0;					// Variable that will store the index of the customer who purchased 
													// the most bottles
	uint64_t maxBottles = 0;						// Variable that will store the largest number of bottle purchases

	uint16_t maxRattleIndex = 0;					// Variable that will store the index of the customer who purchased 
													// the most rattles
	uint64_t maxRattles = 0;						// Variable that will store the largest number of rattle purchases
									
	for (index = 0; index < ellies.numCustomers; index += 1)
	{
		if (allCustomers[index].diapers > maxDiapers)
		{
			maxDiapers = allCustomers[index].diapers;
			maxDiaperIndex = index;
		}
		if (allCustomers[index].bottles > maxBottles)
		{
			maxBottles = allCustomers[index].bottles;
			maxBottleIndex = index;
		}
		if (allCustomers[index].rattles > maxRattles)
		{
			maxRattles = allCustomers[index].rattles;
			maxRattleIndex = index;
		}
	}

	// Print the results of the search.
	if (maxBottles > 0)
	{
		StringPrint(&allCustomers[maxBottleIndex].name);
		printf(" has purchased the most Bottles (%llu)\n", maxBottles);
	}
	else
	{
		printf("no one has purchased any Bottles\n");
	}
	if (maxDiapers > 0)
	{
		StringPrint(&allCustomers[maxDiaperIndex].name);
		printf(" has purchased the most Diapers (%llu)\n", maxDiapers);
	}
	else
	{
		printf("no one has purchased any Diapers\n");
	}
	if (maxRattles > 0)
	{
		StringPrint(&allCustomers[maxRattleIndex].name);
		printf(" has purchased the most Rattles (%llu)\n", maxRattles);
	}
	else
	{
		printf("no one has purchased any Rattles\n");
	}
}

