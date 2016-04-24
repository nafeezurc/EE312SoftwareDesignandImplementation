/*
 * Project6.c
 *
 * Gopika Ajaykumar
 * Sean Tubbs
 * 2:00 - 3:45
 *
 *
 */

// Included Files
#include <stdio.h>
#include <stdint.h>
#include "MazeParams.h"		
#include "Recursion.h"

/*********************************************************************************************************************
                                                 minIt
This function returns the smallest of the elements in an array x[] of n elements (x[0].. x[n-1]) using iteration.
*********************************************************************************************************************/
int minIt(int x[], int n) 
{
	int min = x[0];

	for (uint32_t i = 0; i < n; i += 1)
	{
		if (x[i] < min)
		{
			min = x[i];
		}
	}

	return min;
}

/*********************************************************************************************************************
                                                minRec1
This function returns the smallest of the elements in an array x[] of n elements (x[0].. x[n-1]) recursively using an
"n - 1" type decomposition.
*********************************************************************************************************************/
int minRec1(int x[], int n)
{
	// Base Case: Array of 1 or less
	if (n <= 1)
	{
		return x[0];
	}

	// Inductive Step
	if (x[n - 1] < minRec1(x, n - 1))
	{
		return x[n - 1];
	}
	else
	{
		return minRec1(x, n - 1);
	}
}

/*********************************************************************************************************************
                                                minRec2
This function returns the smallest of the elements in an array x[] of n elements (x[0].. x[n-1]) recursively using an
"n / 2" type of decomposition.
*********************************************************************************************************************/
int minRec2(int x[], int n) 
{
	// Base Case
	if (n <= 1)
	{
		return x[0];
	}

	// Inductive Step
	if (minRec2(x, n / 2) < minRec2(x + (n / 2), n - (n / 2)))
	{
		return minRec2(x, (n / 2));
	}
	else
	{
		return minRec2(x + (n / 2), n - (n / 2));
	}
}

/*********************************************************************************************************************
                                                 sqrtIt
This function calculates and returns the square root of x using iteration. The parameter low_guess is smaller than the
actual square root and the parameter high_guess is larger than the actual square root. The answer will have 15 decimal
digits of accuracy.
*********************************************************************************************************************/
double sqrtIt(double x, double low_guess, double high_guess) 
{
	while (true) 
	{
		double guess = (high_guess + low_guess) / 2;	// Guess the average of low_guess and high_guess.
		double squareOfGuess = guess * guess;

		double error = squareOfGuess - x;				// The error between the square of our guess and x
		if (error < 0)			// Make the error positive.
		{
			error *= -1;
		}

		if (error <= 1E-14)		// This is essentially the square root of x if the condition is met.
		{
			return guess;
		}

		if (x < squareOfGuess)	// guess is too high.
		{
			high_guess = guess;
		}
		else					// guess is too low.
		{
			low_guess = guess;
		}
	} 
}

/*
 * calculate and return the square root of x.
 * The other two parameters are estimates of the square root.
 * low_guess is smaller than the actual square root, and high_guess 
 * is larger than the actual square root.
 * Searching for the square root can be done much like searching
 * for a name in a phone book.
 *
 * Since you can't calculate a square root exactly, for this problem
 * you're required to calculate the answer to 15 decimal digits of
 * accuracy.
 */
double sqrtRec(double x, double low_guess, double high_guess) 
{
	double guess = (high_guess + low_guess) / 2;	// Guess the average of low_guess and high_guess.
	double squareOfGuess = guess * guess;

	double error = squareOfGuess - x;				// The error between the square of our guess and x
	if (error < 0)	// Make the error positive.
	{
		error *= -1;
	}

	// Base Step
	if (error <= 1E-14)
	{
		return guess;		// This is essentially the square root of x.
	}

	// Inductive Step
	if (x < squareOfGuess)	// guess is too high.
	{
		return sqrtRec(x, low_guess, guess);
	}
	else					// guess is too low.
	{
		return sqrtRec(x, guess, high_guess);
	}
}

/*********************************************************************************************************************
                                          strCompare
This function uses recursion to compare strings. The function returns -1 if str1 is less than str2, returns 0 if the
two strings are equal, and returns 1 if str1 is greater than str2. ASCII value is used to compare each character.
*********************************************************************************************************************/
int strCompare(char* str1, char* str2)
{
	if (str1[0] == 0 && str2[0] == 0)	// Both null terminators, strings must be equal
	{
		return 0;
	}
	if (str1[0] < str2[0])				// str1 is "less than" str2
	{
		return -1;
	}
	else if (str1[0] > str2[0])			// str1 is "greater than" str2
	{
		return 1;
	}
	if (str1[0] == str2[0])				// First character of both strings are equal, check rest
	{
		return strCompare(str1 + 1, str2 + 1);
	}
}

/*********************************************************************************************************************
                                         whatLetter
This function takes an input character c, returning a -1 if c is not a letter and the position of c in the alphabet if
c is a letter. This code was given.
*********************************************************************************************************************/
int whatLetter(char c) {
	if (c < 'A') { return -1; }
	if (c > 'z') { return -1; }
	if (c > 'Z' && c < 'a') { return -1; }
	return c & ~32 - 64;
}

/*********************************************************************************************************************
                                              strCompare2
This function takes two strings and compares them using recursion similarly to the strCompare function (see above). 
Unlike strCompare, this function ignores any non-letter characters and ignores the case of the characters.
*********************************************************************************************************************/
int strCompare2(char* str1, char* str2) 
{
	// The character is not a letter, skip it. 
	if (whatLetter(str1[0]) == -1 && str1[0] != 0)
	{
		return strCompare2(str1 + 1, str2);
	}
	if (whatLetter(str2[0]) == -1 && str2[0] != 0)
	{
		return strCompare2(str1, str2 + 1);
	}

	// Both characters are null terminators, so the strings must be equal.
	if ((str1[0] == 0) && (str2[0] == 0))	
	{
		return 0;
	}

	if (whatLetter(str1[0]) < whatLetter(str2[0]))		// str1 is "less than" str2
	{
		return -1;
	}
	else if (whatLetter(str1[0]) > whatLetter(str2[0]))	// str1 is "greater than" str2
	{
		return 1;
	}

	// The first letter of the two strings are equal, check rest
	if (whatLetter(str1[0]) == whatLetter(str2[0]))		
	{
		strCompare2(str1 + 1, str2 + 1);
	}
}

/*********************************************************************************************************************
                                             isEmpty
This function takes in a row and column and, if the row and column are within the confines of the maze array, returns
true if the row and column of the maze is empty (contains a 0) and returns false otherwise.
*********************************************************************************************************************/
bool isEmpty(int row, int col) {
	return (row > 0 && row < MATRIX_SIZE
		&& col > 0 && col < MATRIX_SIZE
		&& maze[row][col] == 0);
}

/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the 
 * value is 1).  Each 0 represents an open space.  
 *
 * Write an recursive solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 *
 * There is a relatively easy recursive solution to this problem, the trick is
 * to think of the solution in the following terms:
 *   "In which direction(s) can I go and find a way out of this maze?"
 * In some cases, you may find yourself in a spot in the maze that there's
 * no way out (at least, not without backtracking). In that case, you'd return "false"
 * since the maze has no solution. In most cases, there's one or more ways out
 * from where you are now. Your key question is simply, "what is the first step I need to take"
 *
 * If you considered going "north", and you had a function that could tell you whether
 * it was possible to get out of the maze starting at the square to the north of your 
 * current position, then you could use this function to determine if you can get out by
 * going north first. Similarly, you could consider going south, east or west, and (recursively)
 * determine if the maze can be solved from any of those locations.
 *
 * With that hint, the base case should be pretty obvious.  In fact,
 * I'd suggest you consider the possibility that the base case is "Yup, I'm already at 
 * the exit!"
 *
 * There is one tricky part to this decomposition.  You need to make certain 
 * that the problem is getting smaller.  The "bigness" or "smallness" of this 
 * problem is the number of squares that have not yet been tested.  Each time
 * you test an adjacent square (making a recursive call to decide if there is a 
 * path to the exit through that square), you'll be reducing  the number of squares 
 * that have not yet been tested.  Eventually, you must have tested all the 
 * squares and so you'll have to have found a way to the exit.
 *
 * The easy way to deal with the tricky part is to leave "bread crumbs" behind.
 * Before you recursively check any (or all) of your neighbors to see if you 
 * can find the exit from there -- drop a bread crumb in your current square.
 * Then, don't ever check to see if you can find the exit using a square
 * with a breadcrumb in it (backtracking into that square would be silly).
 *
 * If you're trying to see if you can find an exit from some square, and all 
 * the adjacent squares are either walls, or have bread crumbs in them, then
 * you already know the answer -- "you can't get to the exit from here". 
 * Pick up your bread crumb and return false.
 * 
 * You can set the value of the current square to "2" to indicate that a bread
 * crumb has been dropped.  Set the square back to 0 when you want to pick 
 * the bread crumb back up.
 * be sure not to change any of the squares that are 1 (the walls).
 *
 * for partial credit, you can leave all your bread crumbs on the floor.
 * for full credit, you must pick up all the bread crumbs EXCEPT those
 * along a path to the exit.
 */

 /*********************************************************************************************************************
											solveMazeRec
 This function solves a maze using recursion. For details on operation and maze, see documentation above.
 *********************************************************************************************************************/
int solveMazeRec(int row, int col) 
{
	// Place a breadcrumb in the current position.
	maze[row][col] = 2;

	// Base Case: Got to the exit
	if (row == (MATRIX_SIZE - 1))
	{
		return 1;
	}

	// Check the four potential directions.
	// Go up.
	if (isEmpty(row - 1, col) && solveMazeRec(row - 1, col))	
	{
		return 1;
	}
	// Go right.
	if (isEmpty(row, col + 1) && solveMazeRec(row, col + 1))
	{
		return 1;
	}
	// Go down.
	if (isEmpty(row + 1, col) && solveMazeRec(row + 1, col))
	{
		return 1;
	}
	// Go left.
	if (isEmpty(row, col - 1) && solveMazeRec(row, col - 1))
	{
		return 1;
	}

	// None of the directions are empty. Backtrack.
	maze[row][col] = 0;
	return 0;
}


/**********************
 * adjacentCell and isOK are functions provided to help you write
 * solveMazeIt()
 */

/*
 * OK, we're currently at maze[row][col] and we're considering moving
 * in direction dir.  
 * Set trow and tcol (local variables inside the calling function)
 * to the row and column that we would move to IF we moved in
 * that direction
 *
 * For example, there are two good ways to use this function.
 * 1. to actually move one step in a direction use:
 *       adjacentCell(row, col, dir, &row, &col);
 *    That will set row and col to new values.  The new values will
 *    be one square away from the old values.
 *
 * 2. to set trow and tcol to a square that is adjacent to row and col use:
 *       adjacentCell(row, col, dir, &trow, &tcol);
 *    That will not change row and col, but will change trow and tcol.
 *    This is useful if you aren't sure if you can actually move in this 
 *    direction yet (e.g., maze[trow][tcol] may be a wall!).  So, you set
 *    trow and tcol, and then check to see if it's OK to move there
 */

 /*********************************************************************************************************************
                                           adjacentCell
 This function enables the user to find the row and column values of an adjacent cell. For details on operation, see
 documentation above. This function was given.
 *********************************************************************************************************************/
void adjacentCell(int row, int col, int dir, int* trow, int* tcol) {
	*trow = row;
	*tcol = col;
	switch(dir) {
	case 0: // UP
		*trow = *trow - 1;
		break;
	case 1: // RIGHT
		*tcol = *tcol + 1;
		break;
	case 2: // DOWN
		*trow = *trow + 1;
		break;
	case 3: // LEFT
		*tcol = *tcol - 1;
		break;
	}
}


/* 
 * return false if there is a wall in the square for row and col
 * return true if it's not a wall.
 */
 /*********************************************************************************************************************
                                                   isOK
 This function takes a row and column and, if the row and column are within the confines of the maze, returns true if
 the row and column of the maze is not a wall (does not contain 1) and returns false otherwise. This function was 
 given.
 *********************************************************************************************************************/
int isOK(int row, int col) {
	return (row > 0 && row < MATRIX_SIZE
		&& col > 0 && col < MATRIX_SIZE
		&& maze[row][col] != 1);
}

/*********************************************************************************************************************
                                                turnRight
This function returns the value of the direction that is one turn to the right. This function was given.
*********************************************************************************************************************/
int turnRight(int dir)
{
	return (dir + 1) % 4;
}

/*********************************************************************************************************************
                                                turnLeft
This function returns the value of the direction that is one turn to the left. This function was given.
*********************************************************************************************************************/
int turnLeft(int dir) {
	return (dir + 3) % 4;
}

/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the value
 * is 1).  Each 0 represents an open space.  
 *
 * write an iterative solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 * The maze can be solved by following the right hand wall.  In fact, there
 * is exactly one path between any two points in the maze (well, between any two
 * points that are not walls).
 *
 * The algorithm is not too bad, although it is certainly not trivial
 * Initially, you'll be headed DOWN (direction 2)
 * Each iteration do one of the following.  Note that only one of two cases
 * can possibly apply (the properties of the maze guarantee that).
 *    case 1: you can move in the current direction
 *       In this case, you should take one step in the current direction
 *       and then turn right.  
 *    case 2: you cannot move in the current direction
 *       If there's a wall directly in front of you, then to follow the right
 *       hand wall, you'd need to turn left (placing your hand on the wall that
 *       used to be directly in front of you).  So, if you discover this case
 *       then turn left.  Don't move, just turn left.
 * If you were walking down a straight corridor (with walls on both sides)
 * then you'd alternate case 1 and case 2.  On the first iteration, you'd
 * take a step, and then turn right (case 1).  This causes you to walk
 * one position down the hallway but then turn to face the wall.
 * On the next iteration, you'll be facing the wall, so you can't walk
 * forward and therefore (case 2) you'll turn left.  On the third iteration
 * you'll be able to walk down the hallway again.
 *
 * If you follow those two cases, then you'll eventually find your way out
 * of the maze.  To confirm that you're making it out, leave a "bread crumb" 
 * behind each square along the path to the exit.
 * For partial credit, place a bread crumb in every square you visit.
 * For full credit, pick up all the breadcrumbs when you backtrack.  
 * Backtracking is when you go back to a square you've already been before.  
 * If the square you're about to step into has a breadcrumb, then pick up 
 * the bread crumb in the square you're at now.
 * You indicate "bread crumbs" by setting the square equal to "2"
 */

 /*********************************************************************************************************************
                                          solveMazeIt
 This function solves a maze using iteration. For details on operation and maze, see above. 
 *********************************************************************************************************************/
void solveMazeIt(int row, int col) {
	int dir = 2; // 0 is up, 1 is right, 2 is down, 3 is left.
	maze[row][col] = 2; // drop a bread crumb in the starting square
	while (row < MATRIX_SIZE - 1) { // the exit is the only open square 
				// in the last row

		int potentialRow, potentialCol;
		adjacentCell(row, col, dir, &potentialRow, &potentialCol);
		if (potentialRow < 0)
		{
			dir = turnLeft(dir);
			adjacentCell(row, col, dir, &potentialRow, &potentialCol);
		}
		if (isOK(potentialRow, potentialCol))
		{
			if (maze[potentialRow][potentialCol] == 2)	// Backtracking
			{
				maze[row][col] = 0;
			}
			row = potentialRow;
			col = potentialCol;
			maze[row][col] = 2;			
			dir = turnRight(dir);
		}
		else
		{	
			dir = turnLeft(dir);
		}
	}
	return;
}

/*********************************************************************************************************************
                                          addTwoMartians
This function takes two Martian objects and adds their respective components together, storing the results in a new
Martian object. The function returns the new Martian object.
*********************************************************************************************************************/
Martian addTwoMartians(Martian m1, Martian m2)
{
	Martian sum;
	sum.pennies = m1.pennies + m2.pennies;
	sum.nicks = m1.nicks + m2.nicks;
	sum.dodeks = m1.dodeks + m2.dodeks;
	return sum;
}

/*********************************************************************************************************************
                                            sumsCoins
This function returns the sum of coins in a Martian object.
*********************************************************************************************************************/
int sumCoins(Martian m)
{
	return (m.dodeks + m.nicks + m.pennies);
}

/*
 * using recursion, with no loops or globals, write a function that calculates the optimal
 * (fewest total coins) change for a given amount of money. Return a Martian struct that indicates
 * this optimal collection of coins.
 */
 /*********************************************************************************************************************
                                            change (one parameter)
 This function uses recursion to calculate the optimal (fewest total coins) change for a given amount of money. The
 function returns a Martian struct that indicates this optimal collection of coins.
 *********************************************************************************************************************/
Martian change(int cents) 
{
	Martian result = { 0, 0, 0 };

	// Base Cases
	if (cents == 0)
	{
		return result;
	}
	if (cents == 1)
	{
		result.pennies = 1;
		return result;
	}
	if (cents == 5)
	{
		result.nicks = 1;
		return result;
	}
	if (cents == 12)
	{
		result.dodeks = 1;
		return result;
	}

	// Induction
	int minimumCoins = -1;
	Martian resultDodeks, resultNicks, resultPennies;
	Martian optimalResult;
	if (cents > 12)			// More than 12 cents
	{
		resultDodeks = { 0, 0, 1 };
		resultDodeks = addTwoMartians(resultDodeks, change(cents - 12));
		minimumCoins = sumCoins(resultDodeks);
		optimalResult = resultDodeks;
	}
    if (cents > 5)			// Greater than 5 cents but less than 12 cents
	{
		resultNicks = { 0, 1, 0 };
		resultNicks = addTwoMartians(resultNicks, change(cents - 5));
		if (minimumCoins == -1)		// If the minimum amount of coins has not been set yet, set it.
		{
			minimumCoins = sumCoins(resultNicks);
			optimalResult = resultNicks;
		}
		else
		{							// The minimum amount of coins has been set. See if there is a new minimum.
			if (sumCoins(resultNicks) < minimumCoins)
			{
				minimumCoins = sumCoins(resultNicks);
				optimalResult = resultNicks;
			}
		}
	}
	// Greater than 1 cent but less than 5 cents
	resultPennies = { 1, 0, 0 };
	resultPennies = addTwoMartians(resultPennies, change(cents - 1));
	if (minimumCoins == -1)		// If the minimum amount of coins has not been set yet, set it.
	{
		minimumCoins = sumCoins(resultPennies);
		optimalResult = resultPennies;
	}
	else
	{							// The minimum amount of coins has been set. See if there is a new minimum.
		if (sumCoins(resultPennies) < minimumCoins)
		{
			minimumCoins = sumCoins(resultPennies);
			optimalResult = resultPennies;
		}
	}

	return optimalResult;
}

 /*********************************************************************************************************************
                                          change (multiple parameters)
 This function uses recursion to calculate the optimal (fewest total coins) change for a given amount of money. The
 function returns a Martian struct that indicates this optimal collection of coins. Unlike the version with one 
 parameter, this function is a more general version where the coins have values a cents and b cents, with a and b
 being algebraic.
 *********************************************************************************************************************/
Martian change(int cents, int nick_val, int dodek_val) 
{
	Martian result = { 0, 0, 0 };

	// Base Cases
	if (cents == 0)
	{
		return result;
	}
	if (cents == 1)
	{
		result.pennies = 1;
		return result;
	}
	if (cents == nick_val)
	{
		result.nicks = 1;
		return result;
	}
	if (cents == dodek_val)
	{
		result.dodeks = 1;
		return result;
	}

	// Induction
	int minimumCoins = -1;
	Martian resultDodeks, resultNicks, resultPennies;
	Martian optimalResult;
	if (cents > dodek_val)			// More than dodek_val cents
	{
		resultDodeks = { 0, 0, 1 };
		resultDodeks = addTwoMartians(resultDodeks, change(cents - dodek_val));
		minimumCoins = sumCoins(resultDodeks);
		optimalResult = resultDodeks;
	}
	if (cents > nick_val)			// Greater than nick_val cents but less than dodek_val cents
	{
		resultNicks = { 0, 1, 0 };
		resultNicks = addTwoMartians(resultNicks, change(cents - nick_val));
		if (minimumCoins == -1)		// If the minimum amount of coins has not been set yet, set it.
		{
			minimumCoins = sumCoins(resultNicks);
			optimalResult = resultNicks;
		}
		else
		{							// The minimum amount of coins has been set. See if there is a new minimum.
			if (sumCoins(resultNicks) < minimumCoins)
			{
				minimumCoins = sumCoins(resultNicks);
				optimalResult = resultNicks;
			}
		}
	}
	// Greater than 1 cent but less than nick_val cents
	resultPennies = { 1, 0, 0 };
	resultPennies = addTwoMartians(resultPennies, change(cents - 1));
	if (minimumCoins == -1)		// If the minimum amount of coins has not been set yet, set it.
	{
		minimumCoins = sumCoins(resultPennies);
		optimalResult = resultPennies;
	}
	else
	{							// The minimum amount of coins has been set. See if there is a new minimum.
		if (sumCoins(resultPennies) < minimumCoins)
		{
			minimumCoins = sumCoins(resultPennies);
			optimalResult = resultPennies;
		}
	}

	return optimalResult;
}

/*********************************************************************************************************************
                                      changeIt (one parameter)
This function uses iteration to calculate the optimal (fewest total coins) change for a given amount of money. The
function returns a Martian struct that indicates this optimal collection of coins.
*********************************************************************************************************************/
Martian changeIt(int cents) 
{
	Martian dodekResult = { 0, 0, 0 };
	Martian nickResult = { 0, 0, 0 };
	int dodekCents = cents;
	int nickCents = cents;
	int dodekCoins = 0;
	int nickCoins = 0;

	// Method 1: Get dodeks first.
	while (dodekCents >= 5)				// More than 5 cents
	{
		while (dodekCents >= 12)		// More than 12 cents
		{
			dodekResult.dodeks = dodekCents / 12;
			dodekCoins += dodekResult.dodeks;
			dodekCents = dodekCents % 12;
		}	
		// Less than 12 cents
		dodekResult.nicks = dodekCents / 5;
		dodekCoins += dodekResult.nicks;
		dodekCents = dodekCents % 5;
	} 
	dodekResult.pennies = dodekCents;
	dodekCoins += dodekResult.pennies;

	// Method 2: Get nicks first.
	while (nickCents >= 5)			// More than 5 cents
	{
		nickResult.nicks = nickCents / 5;
		nickCoins += nickResult.nicks;
		nickCents = nickCents % 5;
	}
	nickResult.pennies = nickCents;
	nickCoins += nickResult.pennies;

	// Get the optimal solution.
	if (dodekCoins > nickCoins)
	{
		return nickResult;			// nickResult is optimal.
	}
	else
	{
		return dodekResult;			// dodekResult is optimal.
	}
}

/*********************************************************************************************************************
                                  changeIt (multiple parameters)
This function uses iteration to calculate the optimal (fewest total coins) change for a given amount of money. The
function returns a Martian struct that indicates this optimal collection of coins. Unlike the version with one 
 parameter, this function is a more general version where the coins have values a cents and b cents, with a and b
 being algebraic.
*********************************************************************************************************************/
Martian changeIt(int cents, int nick_value, int dodek_value) 
{
	Martian dodekResult = { 0, 0, 0 };
	Martian nickResult = { 0, 0, 0 };
	int dodekCents = cents;
	int nickCents = cents;
	int dodekCoins = 0;
	int nickCoins = 0;

	// Method 1: Get dodecks first.
	while (dodekCents >= nick_value)		// More than nick_value cents
	{
		while (dodekCents >= dodek_value)	// More than dodek_value cents
		{
			dodekResult.dodeks = dodekCents / dodek_value;
			dodekCoins += dodekResult.dodeks;
			dodekCents = dodekCents % dodek_value;
		}
		// Less than dodek_value cents
		dodekResult.nicks = dodekCents / nick_value;
		dodekCoins += dodekResult.nicks;
		dodekCents = dodekCents % nick_value;
	}
	dodekResult.pennies = dodekCents;
	dodekCoins += dodekResult.pennies;

	// Method 2: Get nicks first.
	while (nickCents >= nick_value)			// More than nick_value cents
	{
		nickResult.nicks = nickCents / nick_value;
		nickCoins += nickResult.nicks;
		nickCents = nickCents % nick_value;
	}
	nickResult.pennies = nickCents;
	nickCoins += nickResult.pennies;

	// Find the optimal solution.
	if (dodekCoins > nickCoins)
	{
		return nickResult;				// nickResult is the optimal solution.
	}
	else
	{
		return dodekResult;				// dodekResult is the optimal solution.
	}
}
