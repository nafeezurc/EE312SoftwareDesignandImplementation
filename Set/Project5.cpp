/*
 * Project4.c 
 *
 * Gopika Ajaykumar
 * 2:00 - 3:15 PM
 *
 *
 */

// Included Files
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "Set.h"

/*
 * Design NOTES:
 *
 * The design provided in this starter kit assumes
 * (1) empty sets will be represented with length == 0 and elements == nullptr (i.e., address 0)
 * (2) amortized doubling is not used, and capacity is ignored/unused. Functions should assume that 
 * the amount of storage available in the elements[] array is equal to length
 */

/*********************************************************************************************************************
                                                destroySet
destroySet frees the pointer to a Set self's elements. This function was given in the starter kit.
*********************************************************************************************************************/
void destroySet(Set* self) 
{
	free(self->elements);
}

/*********************************************************************************************************************
                                             createEmptySet
createEmptySet takes a pointer to a Set self and makes self an empty set. This function was given in the starter kit.
*********************************************************************************************************************/
void createEmptySet(Set* self) 
{
	self->len = 0;
	self->elements = 0;
}

/*********************************************************************************************************************
                                           createSingletonSet
createSingletonSet takes a pointer to a Set self and an integer x and makes self a singleton set (of length 1) whose 
only element is x. This function was given in the starter kit.
*********************************************************************************************************************/
void createSingletonSet(Set* self, int x) 
{
	self->elements = (int*) malloc(sizeof(int));
	self->elements[0] = x;
	self->len = 1;
}

/*********************************************************************************************************************
                                             createCopySet
createCopySet takes a pointer to a Set self and an pointer to a Set other (in ROM) and makes self a copy of other. 
This function was given in the starter kit.
*********************************************************************************************************************/
void createCopySet(Set* self, const Set* other) 
{
	self->elements = (int*) malloc(other->len * sizeof(int));
	for (int k = 0; k < other->len; k += 1) {
		self->elements[k] = other->elements[k];
	}
	self->len = other->len;	
}

/*********************************************************************************************************************
                                              assignSet
assignSet takes a pointer to a Set self and a pointer to a Set other (in ROM) and, if the pointers are not equal, 
frees the elements of self by calling destroySet (see above) and makes self a copy of other by calling createCopySet
(see above). This function was given in the starter kit.
*********************************************************************************************************************/
void assignSet(Set* self, const Set* other) 
{
	if (self == other) { return; }
	
	destroySet(self);
	createCopySet(self, other);
}

/*********************************************************************************************************************
                                             isMemberSet
isMemberSet takes a pointer to a Set self and an integer x and uses the binary search method to find x in self. If
x is found, the function returns true. If x is not found, the function returns false.
*********************************************************************************************************************/
bool isMemberSet(const Set* self, int x) 
{
	// Perform a binary search.
	int32_t minIndex = 0;						// Left bound of portion of the array being searched
	int32_t maxIndex = self->len - 1;			// Right bound of portion of the array being searched

	while (minIndex <= maxIndex)				// Repeat the process while maxIndex is greater than minIndex.
	{											
		// Set midIndex to the middle of the portion of the array being searched.
		int32_t midIndex = minIndex + (maxIndex - minIndex) / 2;

		if (self->elements[midIndex] == x)		// The element in the middle of the portion of the array being 
		{										// searched is x.
			return true;						// x is an element, so return true to indicate that it was found.
		}
		else if (self->elements[midIndex] < x)	// The element in the middle of the portion of the array being 
		{										// searched is less than x. 
			minIndex = midIndex + 1;			// Search the upper half of the array.
		}
		else									// The element in the middle of the portion of the array being
		{										// searched is greater than x.
			maxIndex = midIndex - 1;			// Search the lower half of the array.
		}
	}
	return false;								// x was not found by the binary search, so return false to indicate
}												// that x is not an element of self.

/*********************************************************************************************************************
                                             insertSet
insertSet takes a pointer to a Set self and an integer x and inserts x into self if x is not already a member of the
set.
*********************************************************************************************************************/
void insertSet(Set* self, int x) 
{
	bool isInserted = false;			// Flag that indicates whether x has been inserted yet

	if (isEmptySet(self))				// self has 0 elements. 
	{
		createSingletonSet(self, x);	// Make self hold one element, x.
		return;							// Return from insertSet.
	}

	// Make sure x is not already a member of self.
	if (isMemberSet(self, x))			// x is already a member of the set.
	{
		return;							// Return from insertSet, doing nothing to self.
	}

	// Allocate a new array of elements that is one element longer than self.
	int* newArray = (int *)malloc((self->len + 1) * sizeof(int));

	int32_t count;						// Loop counter
	for (count = 0; count < self->len; count += 1)
	{
		// Copy the elements of self over to the new array.
		if (self->elements[count] < x)	// The element in self is less than x.
		{
			newArray[count] = self->elements[count];	// Copy the element to the new array.
		}
		else							// The element in self is greater than x.
		{
			newArray[count] = x;		// Insert x into the new array.
			isInserted = true;
			for (int32_t index = count + 1; index <= self->len; index += 1)	// Copy over the remaining elements.
			{
				newArray[index] = self->elements[index - 1];
			}
			break;	// Break out of outer loop.
		}
	}

	if (isInserted == false)	// x has not been inserted yet. Insert x.
	{
		newArray[count] = x;
	}

	// Update self's length.
	self->len += 1;

	// Free self's old elements (before x was inserted).
	destroySet(self);

	// Assign the pointer to the new array to the pointer to self's elements.
	self->elements = newArray;

	// Make sure newArray is not a dangling pointer.
	newArray = nullptr;
}


/*********************************************************************************************************************
                                            removeSet
removeSet takes a pointer to a Set self and an integer x and, if x is a member of self, removes x from self.
*********************************************************************************************************************/
void removeSet(Set* self, int x)
{
	// Make sure x is a member of self.
	if (!isMemberSet(self, x))			// x is not a member of self.
	{
		return;							// Return, doing nothing to self.
	}

	int32_t count;						// Loop Counter
	for (count = 0; count < self->len; count += 1)
	{
		if (self->elements[count] == x)				// The element in self is equal to x.										// The element in self is equal to x
		{
			for (int32_t index = count; index < (self->len - 1); index += 1)
			{
				self->elements[index] = self->elements[index + 1];
			}
			break;									// Break from the outer loop.
		}
	}

	// Update self's length.
	self->len -= 1;
}

/*********************************************************************************************************************
                                           displaySet
displaySet takes a pointer to a Set self (in ROM) and displays the contents of self on the console. This function was 
given in the starter kit.
*********************************************************************************************************************/
void displaySet(const Set* self) {
	int k;
	
	printf("{");

	if (isEmptySet(self)) { 
		printf("}"); 
	}
	else {
		for (k = 0; k < self->len; k += 1) {
			if (k < self->len - 1) {
				printf("%d,", self->elements[k]);
			} else {
				printf("%d}", self->elements[k]);
			}
		}
	}
}

/*********************************************************************************************************************
                                          isEqualToSet
isEqualToSet takes a pointer to a Set self (in ROM) and a pointer to a Set other (in ROM) and determines if the two
sets are equal. If the two sets are equal, the function returns true. If the two functions are unequal, the function
returns false.
*********************************************************************************************************************/
bool isEqualToSet(const Set* self, const Set* other) 
{
	if ((isEmptySet(self)) && (isEmptySet(other)))	// Both sets are empty sets.
	{
		return true;
	}
	if (self->len != other->len)					// The two sets are of two different lengths and are not equal.
	{
		return false;
	}

	// Check if self and other have the same elements.
	for (int32_t count = 0; count < self->len; count += 1)
	{
		if (self->elements[count] != other->elements[count])	// Two unequal elements have been found.
		{
			return false;										// Return false to indicate that the two sets are 
		}														// unequal.
	}
	return true;												// All the elements were equal. Return true to 
}																// indicate that the two sets are equal.

/*********************************************************************************************************************
                                          isSubsetOf
isSubsetOf takes a pointer to a Set self (in ROM) and a pointer to a Set other (in ROM) and determines whether self
is a subset of other (i.e. whether for any element x in self, x is in other). If self is a subset of other, then 
the function returns true. If self is not a subset of other, then the function returns false.
*********************************************************************************************************************/
bool isSubsetOf(const Set* self, const Set* other) 
{
	// If self is an empty set or if self and other are equal, then self is a subset of other.
	if ((isEmptySet(self)) || (isEqualToSet(self, other)))
	{
		return true;
	}
	// If other is an empty set, self is bigger than other, or self and other have equal lengths but are not equal,
	// then self cannot be a subset of other.
	if ((isEmptySet(other)) || (self->len > other->len) || (self->len == other->len))
	{
		return false;
	}

	// Check if self's elements are in other's elements.
	int32_t selfIndex = 0, otherIndex = 0;							// Indeces used to traverse the sets
	while ((selfIndex < self->len) && (otherIndex < other->len))	// While the end of neither element has been
	{                                                               // reached, continue.
		if (self->elements[selfIndex] == other->elements[otherIndex])	// Two equal elements have been found.
		{																// Check consecutive elements.
			selfIndex += 1;
			otherIndex += 1;
		}
		else
		{
			if (self->elements[selfIndex] > other->elements[otherIndex])	// Current element of self is greater
			{																// than other's element. Go to the next
				otherIndex += 1;											// element of other.
			}
			else														// Current element of self is less than
			{															// other's element. This cannot occur if
				return false;											// self is a subset, so self is not a subset.
			}															// Return false to indicate that self is not
		}																// a subset of other.
	}
	return true;														// self is a subset. Return true to indicate
}																		// that self is a subset of other.

/*********************************************************************************************************************
                                             isEmptySet
isEmptySet takes a pointer to a Set self (in ROM) and determines whether self is empty (i.e. whether its length is 0).
If self is empty, the function returns true. If self is nonempty, the function returns false. This function was given 
in the starter kit.
*********************************************************************************************************************/
bool isEmptySet(const Set* self) 
{
	return self->len == 0;
}

/*********************************************************************************************************************
                                         intersectFromSet
intersectFromSet takes a pointer to a Set self (in ROM) and a pointer to a Set other (in ROM) and makes self the
intersection of self and other (i.e. fills self with the elements both self and other have in common).
*********************************************************************************************************************/
void intersectFromSet(Set* self, const Set* other) 
{
	if (isEqualToSet(self, other) || isEmptySet(self)) // If self and other are equal or if self is an empty set, then 
	{											       // self is already the intersection of self and other. Return						
		return;										   // from intersectFromSet without changing self.
	}
	if (isEmptySet(other))							 // If other is an empty set, then self should become an empty set.
	{
		createEmptySet(self);
		return;									   	 // Return from intersectFromSet.
	}

	// Find intersecting elements.
	int* intersection = (int*)malloc(self->len * sizeof(int));

	int32_t selfIndex = 0, otherIndex = 0, interIndex = 0;			// Indeces for traversing the sets/arrays
	while ((selfIndex < self->len) && (otherIndex < other->len))	// While the end of neither set has been reached
	{
		if (self->elements[selfIndex] < other->elements[otherIndex])	// If the current element of self is less than
		{																// the current element of other, go to the 
			selfIndex += 1;												// next element of self.
		}
		else if (self->elements[selfIndex] > other->elements[otherIndex]) // If the current element of other is less
		{																  // than the current element of self, go to
			otherIndex += 1;											  // the next element of other.
		}
		else													// Equal elements from self and other have been found.
		{
			intersection[interIndex] = self->elements[selfIndex];	// Put the common elements into the intersection.
			interIndex += 1;	// Update the index of intersection (also the length of the intersection array).
			selfIndex += 1;		// Go to the next element of self.
			otherIndex += 1;	// Go to the next element of other.
		}
	}

	// Free old elements of self.
	destroySet(self);

	// Assign the pointer to the intersection set to the pointer to self's elements.
	self->elements = intersection;

	// Make sure intersection is not a dangling pointer.
	intersection = nullptr;

	// Update self's length.
	self->len = interIndex;
}

/*********************************************************************************************************************
                                             subtractFromSet
subtractFromSet takes a pointer to a Set self (in ROM) and a pointer to a Set other (in ROM) and makes self  
self - other (i.e. removes the elements of self that both self and other have in common).
*********************************************************************************************************************/
void subtractFromSet(Set* self, const Set* other) 
{
	if (isEmptySet(self) || isEmptySet(other))	// If either self or other is an empty set, do not change self.
	{
		return;
	}
	
	// Remove the elements in self that are also in other.
	int* subtractedSet = (int*)malloc(self->len * sizeof(int32_t));

	int32_t selfIndex = 0, otherIndex = 0, subIndex = 0;			// Indeces used to traverse the sets/arrays
	while ((selfIndex < self->len) && (otherIndex < other->len))	// While the end of neither set has been reached
	{
		if (self->elements[selfIndex] < other->elements[otherIndex])	// If the current element of self is less than
		{																// the current element of other, put the  
			subtractedSet[subIndex] = self->elements[selfIndex];		// element into subtractedSet and go to the 
			subIndex += 1;												// next element of self.
			selfIndex += 1;
		}
		else if (self->elements[selfIndex] > other->elements[otherIndex]) // If the current element of other is less
		{																  // than the current element of self, go to
			otherIndex += 1;											  // the next element of other.
		}
		else													// Equal elements from self and other have been found.
		{
			selfIndex += 1;		// Go to the next element of self.
			otherIndex += 1;	// Go to the next element of other.
		}
	}
	
	while (selfIndex < self->len)
	{
		subtractedSet[subIndex] = self->elements[selfIndex];
		selfIndex += 1;
		subIndex += 1;
	}

	// Free the old elements of self.
	destroySet(self);

	// Assign the pointer to subtractedSet to the pointer to self's elements.
	self->elements = subtractedSet;

	// Update self's length.
	self->len = subIndex;

	// Make sure that subtractedSet is not a dangling pointer.
	subtractedSet = nullptr;
}

/*********************************************************************************************************************
                                              unionInSet
unionInSet is a function that takes a pointer to a Set self (in ROM) and a pointer to a Set other (in ROM) and makes
self the union of self and other (i.e. fills it with elements from both of the sets, without repeat).
*********************************************************************************************************************/
void unionInSet(Set* self, const Set* other) 
{
	if (isEmptySet(other))	// other is an empty set so self is the union of self and other.
	{
		return;				// Return from unionInSet without changing self.
	}
	if (isEmptySet(self))	// self is an empty set so other is the union of self and other.
	{
		assignSet(self, other);	// Assign other to self.
		return;					// Return from unionInSet.
	}

	// Find the union of the two sets.
	int* unionOfSet = (int*)malloc((self->len + other->len) * sizeof(int32_t));

	uint32_t selfIndex = 0, otherIndex = 0, unionIndex = 0;			// Indeces used to traverse the sets/arrays
	while ((selfIndex < self->len) && (otherIndex < other->len))	// While the end of neither set has been reached
	{
		if (self->elements[selfIndex] < other->elements[otherIndex])	// If the current element of self is less than
		{																// the current element of other, put the  
			unionOfSet[unionIndex] = self->elements[selfIndex];			// element into unionOfSet and go to the next
			unionIndex += 1;											// element of self.
			selfIndex += 1;											
		}
		else if (self->elements[selfIndex] > other->elements[otherIndex]) // If the current element of other is less
		{																  // than the current element of self, put the
			unionOfSet[unionIndex] = other->elements[otherIndex];		  // element into unionOfSet and go to the 
			unionIndex += 1;											  // next element of other.
			otherIndex += 1;											  
		}
		else													// Equal elements from self and other have been found.
		{
			unionOfSet[unionIndex] = self->elements[selfIndex];	// Put the common elements into the union.
			unionIndex += 1;	// Update the index of union (also the length of the union array).
			selfIndex += 1;		// Go to the next element of self.
			otherIndex += 1;	// Go to the next element of other.
		}
	}

	while (selfIndex < self->len)								// While the end of self has not been reached, repeat.
	{
		unionOfSet[unionIndex] = self->elements[selfIndex];		// Copy the rest of the elements of self into 
		unionIndex += 1;										// unionOfSet.
		selfIndex += 1;
	}

	while (otherIndex < other->len)								// While the end of other has not been reached, repeat.
	{
		unionOfSet[unionIndex] = other->elements[otherIndex];	// Copy the rest of the elements of other into 
		unionIndex += 1;										// unionOfSet.
		otherIndex += 1;
	}

	// Free the old elements of self.
	destroySet(self);

	// Assign the pointer to the union to the pointer to set's elements.
	self->elements = unionOfSet;

	// Update the length of self.
	self->len = unionIndex;

	// Make sure unionOfSet is not a dangling pointer.
	unionOfSet = nullptr;
}