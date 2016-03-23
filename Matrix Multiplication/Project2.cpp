/* Project2.cpp
 * Gopika Ajaykumar
 * EE312, Spring 2015
 * Project 2, Matrix Multiplication
 */

#include <stdio.h>
#include <stdint.h>
#include "MatrixMultiply.h"

/*
 * An example routine demonstrating the use of row-major ordering
 *
 * Not used in the solution
 *
 * given two matrices a and b, where the number of rows in b is equal
 * to the number of columns in a,
 * and given a row number 'row' and a column number 'col',
 * calculate the dot product of the row'th row from a and the col'th column from b
 *
 * recall the dot product is the sum of the element-by-element product of
 * the two vectors.
 */
double dotProduct(double a[], const uint32_t a_rows, const uint32_t a_cols,
    /* a is a matrix with a_rows rows and a_cols columns */
    double b[], const uint32_t b_cols,
    /* b is also a matrix.  It has a_cols rows and b_cols columns */
    uint32_t row, // an index of a row of a
    uint32_t col) { // an index of a column of b

        double sum = 0.0; // the result of the dot product is stored here
        uint32_t k = 0;
        while (k < a_cols) { // recall: a_cols == b_rows
            /* we need to multiply a[row, k] and b[k, col] and add that to sum */
            sum = sum + a[(row * a_cols) + k] * b[k * b_cols + col];
            /* recall a[i,j] is stored at a[(i * a_cols) + j] and b[i, j] is at b[(i * b_cols) + j] */
            k += 1;
        }

        return sum;
}

/*****************************************************************
                  multiplyMatrices
This function takes two pointers a and b (which point to arrays of
doubles symbolizing matrices) and the rows and columns of the 
respective "matrices" and multiplies the two matrices together, 
assigning the resulting matrix to c, a pointer to an array of 
doubles.
*****************************************************************/
void multiplyMatrices(
        double a[],	// Pointer to "matrix" A
        const uint32_t a_rows,	// Rows in A
        const uint32_t a_cols,	// Columns in A
        double b[],	// Pointer to "matrix" B
        const uint32_t b_cols,	// Columns in B
        double c[]) {	// Pointer to result matrix "C"; Start of function
	for (uint8_t rowIndex = 0; rowIndex < a_rows; rowIndex += 1)	// Loops through rows
	{
		for (uint8_t colIndex = 0; colIndex < b_cols; colIndex += 1)	// For each row, loops through columns
		{
			c[rowIndex * b_cols + colIndex] = 0;	// Initialize current element of matrix C to zero.
			for (uint8_t sumIndex = 0; sumIndex < a_cols; sumIndex+= 1)	// For each row and column, summing loop
			{
				c[rowIndex * b_cols + colIndex] += a[rowIndex * a_cols + sumIndex] * b[sumIndex * b_cols + colIndex];
			}	// End of summing loop
		}	// End of column loop
	}	// End of row loop
}

#define READY_FOR_STAGE2
#ifdef READY_FOR_STAGE2

/* these three variables are used in Stage 2 to implement allocateSpace */
#define TOTAL_SPACE 10000			// Total size of memory pool
double memory_pool[TOTAL_SPACE];	// The array that serves as a memory pool for memory allocation
uint32_t top_of_pool = 0;			// Holds the current index of the element not yet allocated

/*****************************************************************
                      allocateSpace
This function allocates space for a matrix using a global array
called memory_pool. The function uses the parameter size to
increment the global top_of_pool variable, which enables
traversing through the array as memory needs to be allocated.
The function returns a pointer to the first element not yet
allocated by the function.
*****************************************************************/
PtrDouble allocateSpace(uint32_t size) {
	PtrDouble pointer = &memory_pool[top_of_pool];
	if ((top_of_pool + size) <= 10000)	// Makes sure that the user does not step out of the bounds of the memory_pool array
		top_of_pool += size;
	return pointer;
}

/*****************************************************************
             multiplyMatrixChain
This function calls the multiplyMatrices and allocateSpace
functions in order to implement the multiplication of multiple
matrices. See comments in function below for details on the
implementation. The result of the multiplication is assigned to
out, a pointer to an array of doubles.
*****************************************************************/
void multiplyMatrixChain(
    PtrDouble matrix_list[],	// Array of pointers to "matrices" (i.e. arrays of doubles)
    uint32_t rows[],			// Array of number of rows for each matrix to be multiplied, corresponds to matrix_list
    uint32_t cols[],			// Array of number of columns for each matrix to be multiplied, corresponds to matrix_list
    uint32_t num_matrices,		// Number of matrices to be multiplied
    double out[],				// Pointer to an array of doubles, final result is assigned to this array
    uint32_t out_rows,			// Number of rows of final resulting matrix
    uint32_t out_cols) {		// Number of columns of final resulting matrix

	/* The loop works by continually multiplying the a_mat
	* and b_mat matrix to obtain the c_mat matrix.
	* After each iteration, the previous "c_mat" becomes the
	* next "a_mat" matrix.  the b_mat matrix will be the
	* next matrix in the matrix_list array.
	*
	* For example, on the first time through a_mat is matrix_list[0]
	* b_mat is matrix_list[1]
	* c_mat (the output) will be a temporary matrix that you must allocate
	* space for.
	* After multiplying a_mat and b_mat together, you need to
	* take the result and multiply it by matrix_list[2],
	* so a_mat will be set to c_mat, and b_mat will be matrix_list[2]
	* you must again allocate a new temporary array to hold the result.
	*
	* There's no guarantee that the temporary arrays are the same size,
	* so there's no practical way to reuse the temporary arrays.
	* Therefore, allocate a new array each iteration.  We'll clean up when we're done.
	*/

	double* a_mat = matrix_list[0];
	uint32_t a_rows = rows[0]; // number of rows in a matrix
	uint32_t a_cols = cols[0]; // number of cols in a matrix
	uint32_t next_src = 1; // index of next matrix to multiply by
	double* b_mat;
	double* c_mat = 0;
	uint32_t b_cols;
	while (next_src < num_matrices) {
		/* Each iteration of the loop perform the following
		 * set c_mat = a_mat * b_mat where
		 * a_mat is the result from the previous iteration of the loop (or matrix_list[0] on the first iteration)
		 * b_mat is the next matrix in the matrix list
		 * c_mat is allocated space (using our handy-dandy memory allocator function) */
		b_mat = matrix_list[next_src];
		b_cols = cols[next_src];
		c_mat = allocateSpace(a_rows * b_cols);

		if (next_src < num_matrices - 1) {
			multiplyMatrices(a_mat, a_rows, a_cols, b_mat, b_cols, c_mat);
			a_mat = c_mat;
			a_cols = b_cols;
		}
		else { // SPECIAL CASE: The last matrix multiplication. Final result will be placed in out 
			multiplyMatrices(a_mat, a_rows, a_cols, b_mat, b_cols, out);
		}
		next_src += 1;
	}

	/* deallocate all memory (this only takes one line, 'cause we have such a simple memory allocator) */
	top_of_pool = 0;	// Back to the beginning
}

#endif /* READY_FOR_STAGE_2 */