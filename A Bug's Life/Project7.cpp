/*
Gopika Ajaykumar
Project 7: Bugs
Simulates Life
EE 312 Spring 2016
*/

// Included Files
#include "Bugs.h"
#include <random>
using std::rand;

// Global Variables
int time_step = 0;
int average_age = 0;
int average_generation = 0;
int percent_straight;
int percent_right;
int percent_left;
int percent_back;
int total_left = 0;
int total_right = 0;
int total_straight = 0;
int total_back = 0;

/**********************************************************************************************************************
                                                   createBug
This function is used by createWorld to generate random bugs. The function was given as starter code.
**********************************************************************************************************************/
Bug createBug(int x, int y) 
{
	Bug b;
	b.age = 0;
	b.generation = 0;
	b.x = x;
	b.y = y;
	b.dir = rand() % 8;
	b.health = START_HEALTH;
	for (int k = 0; k < 8; k += 1) 
	{
		if (DISTRIBUTED_GENES) 
		{
			b.genes[k] = GENE_TOTAL / 8;
		} else 
		{
			if (k == 0) 
			{
				b.genes[k] = GENE_TOTAL;
			} else
			{
				b.genes[k] = 0;
			}
		}
	}
	total_straight += b.genes[0];
	total_right += b.genes[1] + b.genes[2] + b.genes[3];
	total_left += b.genes[5] + b.genes[6] + b.genes[7];
	total_back += b.genes[4];
	return b;
}

/**********************************************************************************************************************
                                                 createWorld
This function is used to initialize the world. The function was given as starter code.
**********************************************************************************************************************/
void createWorld(void) 
{
	/* empty the world */
	for (int i = 0; i < WORLD_SIZE; i += 1) 
	{
		for (int j = 0; j < WORLD_SIZE; j += 1) 
		{
			world[i][j] = EMPTY;
		}
	}

	/* create food */
	for (int k = 0; k < INITIAL_FOOD; k += 1) 
	{
		int x = rand() % WORLD_SIZE;
		int y = rand() % WORLD_SIZE;
		world[x][y] = FOOD;
	}
	
	/* create bugs */
	while (bug_list.size() < INITIAL_BUGS) 
	{
		int x = rand() % WORLD_SIZE;
		int y = rand() % WORLD_SIZE;
		if (world[x][y] == EMPTY) 
		{
			world[x][y] = bug_list.size();
			bug_list.push_back(createBug(x, y));
		}
	}	
}

 /**********************************************************************************************************************
                                                    wrap
 This function ensures that bugs that "fall off" the edge of the world automatically reappear on the opposite edge.
 The function was given as starter code.
 **********************************************************************************************************************/
int wrap(int y) 
{
	if (y < 0) { return WORLD_SIZE - 1; }
	else if (y == WORLD_SIZE) { return 0; }
	else { return y; }
}

/**********************************************************************************************************************
                                                    newX
This function returns the new x-coordinate a bug would have if it were to take a step from x-coordinate x in direction
dir. The function was given as starter code.
**********************************************************************************************************************/
int newX(int x, int dir) 
{
	switch (dir) 
	{
		case 0: // move up
		case 1:
		case 7:
			return wrap(x - 1);
			break; // not reached
		case 3: // move down
		case 4:
		case 5:
			return wrap(x + 1);
			break; // not reached
		case 2: // move left or right
		case 6:
			return x;
	}
	/* NOT REACHED */
	return 0;
}

/**********************************************************************************************************************
                                                    newY
This function returns the new y-coordinate a bug would have if it were to take a step from y-coordinate y in direction
dir. The function was given as starter code.
**********************************************************************************************************************/
int newY(int y, int dir)
{
	switch (dir) 
	{
	case 1:
	case 2:
	case 3:
		return wrap(y + 1);
	case 5:
	case 6:
	case 7:
		return wrap(y - 1);
	case 0:
	case 4:
		return y;
	}
	/* NOT REACHED */
	return 0;
}

 /**********************************************************************************************************************
                                                   addFood
 This function adds NUM_FOOD food objects in random locations. The function was given as starter code.
 **********************************************************************************************************************/
void addFood(void) 
{
	for (int food = 0; food < NUM_FOOD; food += 1) 
	{
		int x = rand() % WORLD_SIZE;
		int y = rand() % WORLD_SIZE;
		switch (world[x][y]) 
		{
		case EMPTY: // empty, add the food
			world[x][y] = FOOD;
			break;
		case FOOD: // already has food, do nothing
			break;
		default: // bug, feed the bug
			bug_list[world[x][y]].health += EAT_HEALTH;
		}
	}	
}

 /* move bugs 
 * if they move onto food, feed them
 * if they move onto another bug, no big deal.  Two bugs
 * can be in the same square.  Let's say bug#2 and bug #6 are in the same
 * square.  Well, both bugs are still in the bug_list, so they
 * will still be able to move next time step.
 * Since the world[][] array can only hold either a 2 or a 6, we'll just
 * put one of them there.  The easiest thing to do is just put which
 * ever bug moves into the square first into the square.  When the next
 * bug moves into the square, update his x and y (in bug_list) so that
 * he is in the square, leave the other bug alone (i.e., it's also in
 * the same square) and then set world[x][y] = 6 (the bug # of the second
 * bug).  No harm done.  The graphics will draw only the second bug
 * but who cares.  The next time step they'll move into different squares
 * (probably) and no one will notice.
 * NOTE: only the first bug to move into the square gets to eat the food
 * there.
 */
 /**********************************************************************************************************************
	                                         moveBugs
 This function moves each bug and updates the bug's stats and the total stats as necessary. See above for 
 implementation details.
 **********************************************************************************************************************/
void moveBugs(void) 
{
	int total_age = 0;
	int total_gen = 0;
	
	/* update each bug in turn (but don't kill them) */
	for (int k = 0; k < bug_list.size(); k += 1)	// Repeat for each bug.
	{

		// Calculate the new position for the bug.
		int old_x = bug_list[k].x;					// Old x location
		int old_y = bug_list[k].y;					// Old y location
		int new_x = newX(old_x, bug_list[k].dir);	// New x location
		int new_y = newY(old_y, bug_list[k].dir);	// New y location
		
		// Check the new position to see if there’s food there.
		if (world[new_x][new_y] == FOOD)			// If the bug stepped on food, increase the bug's health.
		{
			bug_list[k].health += EAT_HEALTH;
		}
		
		// Calculate a new direction for the bug.
		int randVal = rand() % GENE_TOTAL;				// randVal is a random number between 0 and GENE_TOTAL.
		int choice = 0;									// Choice number
		int compareValue = 0;
		for (choice = 0; choice < 8; choice += 1) 
		{
			compareValue += bug_list[k].genes[choice];
			if (randVal < compareValue) 
			{
				bug_list[k].dir = (bug_list[k].dir + choice) % 8;
				break;
			}
		}

		// Update the x and y coordinates for the bug.
		bug_list[k].x = new_x;
		bug_list[k].y = new_y;

		// Update the world[][], change the old position to EMPTY, and set the new position equal to the bug’s number.
		world[old_x][old_y] = EMPTY;
	
		world[new_x][new_y] = k;

		// Subtract MOVE_HEALTH from the bug.
		bug_list[k].health -= MOVE_HEALTH;

		// Done in starter code: update this bug's age for statistics 
		bug_list[k].age += 1;
		total_age += bug_list[k].age;
		total_gen += bug_list[k].generation;

	}	// end for loop

	// Done in starter code: some statistics...
	average_age = total_age / bug_list.size();
	average_generation = total_gen / bug_list.size();
}

/* look through the bug_list for dead bugs (bugs with zero 
 * health) and kill them
 *
 * To kill a bug, mark its position in the world as EMPTY
 * then remove it from the bug_list.
 *
 * To remove a bug from the bug_list, use the following simple
 * trick.  Swap the bug with the last bug in the list, then
 * do "pop_back" on the bug_list vector.
 * Hey, since we're going to do pop_back anyway, we don't really
 * need to swap with the last bug.  We just need to copy the 
 * last bug into our position.
 *
 * WATCH OUT: there's a trap.  Be careful to account for the possibility
 * that the "last bug" is also dead.  Don't miss out on killing it
 * because you moved that bug forward in the bug_list
 *
 * Be sure to update the global statistics (total_straight, total_right, total_left and total_back)
 * for each bug that you kill.
 */
 /**********************************************************************************************************************
                                                  killDeadBugs
 This function kills dead bugs (with 0 health) by removing them from bug_list and from the world. See above for 
 implementation details.
 **********************************************************************************************************************/
void killDeadBugs(void) 
{
	int k = bug_list.size() - 1;
	while (k >= 0) 
	{
		if (bug_list[k].health == 0)						// If the bug is dead, "kill it."
		{
			world[bug_list[k].x][bug_list[k].y] = EMPTY;	// Mark the position of the bug empty.

			// Update the global statistics.
			total_straight -= bug_list[k].genes[0];
			total_right -= (bug_list[k].genes[1] + bug_list[k].genes[2] + bug_list[k].genes[3]);
			total_left -= (bug_list[k].genes[5] + bug_list[k].genes[6] + bug_list[k].genes[7]);
			total_back -= bug_list[k].genes[4];

			// Now move the last bug to the dead bug's position in bug_list. Then remove the last element of 
			// bug_list.
			bug_list[k] = bug_list.back();
			if (k != bug_list.size() - 1) 
			{
				world[bug_list.back().x][bug_list.back().y] = k;
			}
			bug_list.pop_back();
		}
		k -= 1;
	}
}

/* Look at all the bugs in the bug_list.  For each bug
 * that has enough health to reproduce, reproduce a new bug.
 * Put the new bug in the same square and have it head in the
 * same direction.
 *
 * Give both the new bug and the old bug 1/2 the energy of the
 * old bug.
 *
 * Mutate the genes of the new bug as follows.  Choose two genes
 * at random.  Increment one of those genes (by one) and decrement
 * the other gene (by one).
 * YOU MUST ensure 
 * 1. that no genes ever get smaller than zero
 * 2. the total of the eight genes is always GENE_TOTAL
 *
 * Be sure to put the new bug in the bug_list (no need to update
 * the world, since you have two bugs in the same square anyway).
 */
 /**********************************************************************************************************************
                                        reproduceBugs
 This function creates a new bug and updates its statistics whenever its parent is ready to reproduce. See above
 for implementation details.
 **********************************************************************************************************************/
void reproduceBugs(void) 
{
	int num_bugs = bug_list.size();
	for (int k = 0; k < num_bugs; k += 1)				// Repeat for all bugs.
	{
		if (bug_list[k].health >= REPRODUCE_HEALTH) 
		{
			// Create a new bug.
			Bug b; 

			// Make the new bug a copy of the parent, but with a generation count one greater than the parent's and
			// an age of 0.
			b = bug_list[k];
			b.generation += 1;
			b.age = 0;

			// Update health.
			bug_list[k].health /= 2;
			b.health = bug_list[k].health;

			// Mutate the new bug's genes.
			int i = rand() % 8;					// i is a number between 0 and 7.
			while (b.genes[i] == 0)
			{
				i = rand() % 8;
			}
			b.genes[i] -= 1;
			
			// Sanity check: Ensure we did not make the genes less than 0.
			if (b.genes[i] < 0)
			{
				abort();
			}

			int j = rand() % 8;					// j is a number between 0 and 7.
			b.genes[j] += 1;

			// Sanity check: Ensure we did not make the total number of genes greater or less than GENE_TOTAL.
			int totalGenes = 0;					// Holds total number of genes for the bug
			for (int i = 0; i < 8; i += 1)
			{
				totalGenes += b.genes[i];
			}
			if (totalGenes != GENE_TOTAL)
			{
				abort();
			}

			// Done in starter code: update global statistics based on the new genes in b 
			total_straight += b.genes[0];
			total_right += b.genes[1] + b.genes[2] + b.genes[3];
			total_left += b.genes[5] + b.genes[6] + b.genes[7];
			total_back += b.genes[4];

			// Done in starter code: add b to the global bug list 
			bug_list.push_back(b);
		}	// end if
	}	// end for
}

/**********************************************************************************************************************
                                                 timeStep  
This function updates the world approximately every 20ms. The function was given in the starter code. During a time 
step, each bug moves one space, and may eat or turn. Dead bugs are removed from the bug_list and healthy bugs give 
birth to new bugs.
**********************************************************************************************************************/
void timeStep() 
{
	time_step += 1;
	addFood();
	moveBugs();
	killDeadBugs();
	reproduceBugs();
	percent_straight = 100 * total_straight / bug_list.size() / GENE_TOTAL;
	percent_left = 100 * total_left / bug_list.size() / GENE_TOTAL;
	percent_right = 100 * total_right / bug_list.size() / GENE_TOTAL;
	percent_back = 100 * total_back / bug_list.size() / GENE_TOTAL;
}	
