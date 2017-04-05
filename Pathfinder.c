#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "QueueInterface.h"

#define MINROWS 2
#define MINCOLS 2
#define MAXROWS 20
#define MAXCOLS 20
#define BUFFER_SIZE 500

char path[BUFFER_SIZE];	  // The path string that is printed at the end.
char buffer[BUFFER_SIZE]; // Stores the current array index.

// Function Prototypes
void SetupPath(int rows, int cols, char M[rows][cols]); // Sets up the path finding algorithms.
void FindPath(Queue * Container, int rows, int cols, ItemType Cells[rows][cols]); // Finds the path
ItemType FindStart(int rows, int cols, ItemType Cells[rows][cols]); // Finds the start cell
void InsertAdjCells(Queue * Container, ItemType currentItem, int rows, int cols, // Inserts the appropriate adjacent cells...
	ItemType Cells[rows][cols]);												 // ...onto the queue.
void PrintPath(int rows, int cols, ItemType Cells[rows][cols]);	// Prints the path onto the stream.
bool found = 0; // Stores whether or not the path has been found.


// Main function
int main() 
{
	// Stores the number rows and columns in the matrix.
	int rows, cols;

	// Get the rows and columns for the matrix as long as they stay within the bounds.
	do {
		scanf("%d %d", &cols, &rows);
	} while (!(rows <= MAXROWS && rows >= MINROWS && cols <= MAXCOLS && cols >= MINCOLS));
	
	// Get the matrix from the user. Note: Add 1 to columns for newline character and terminating char.
	char M[rows][cols + 1];
	for (int i = 0; i < rows; i++)
	{
		scanf("%s", M[i]);
	}

	// Setup the path finding algorithm using the rows, columns, and matrix given by the user.
	SetupPath(rows, cols + 1, M);
	return 0;
}

// Sets up the path finding algorithm using the rows, columns, and matrix gien by the user.
void SetupPath(int rows, int cols, char M[rows][cols])
{
	// Initialize the container queue that will store the unoccupied cells.
	Queue * Container = (Queue*)malloc(sizeof(Queue));
	InitializeQueue(Container);

	// Initialize a 2D ItemType array that stores all of the characters as ItemTypes.
	ItemType Cells[rows][cols-1];
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols - 1; j++)
		{
			Cells[i][j].value = M[i][j];
			Cells[i][j].visited = 0;
			Cells[i][j].row = i;
			Cells[i][j].col = j;
		}
	}

	ItemType start = FindStart(rows, cols - 1, Cells); // Get the starting cell from the Cells array
	Insert(Cells[start.row][start.col], Container); // Insert the starting cell into the Container
	FindPath(Container, rows, cols - 1, Cells); // Find the path
	PrintPath(rows, cols - 1, Cells); // Print the path if one was found.
}

// Prints the path to the stream.
void PrintPath(int rows, int cols, ItemType Cells[rows][cols])
{
	if (found) { // If the path was found, print the path.
		printf("A Path:\n");
		puts(path);

		// Print the modified matrix.
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				printf("%c", Cells[i][j].value);
			}
			printf("\n");
		}
	}
	else { // Otherwise, print that no path was found.
		printf("No Path\n");
	}
}

// Finds the start cell in the matrix and returns it
ItemType FindStart(int rows, int cols, ItemType Cells[rows][cols])
{
	// Stores the starting cell.
	ItemType start;

	// Iterate through the matrix and if the start cell is found,
	// initialize the start cell.
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ItemType current = Cells[i][j];
			if (current.value == 's') {
				start.value = 's';
				start.row = i;
				start.col = j;
				start.visited = 0;
			}
		}
	}
	return start;
}

// Finds the path using the Container, rows, columns, and Cells provided by the user.
void FindPath(Queue * Container, int rows, int cols, ItemType Cells[rows][cols])
{
	ItemType front; // Stores the front of the Container.
	while (Empty(Container) == 0) // While the Container isn't Empty
	{
		Remove(Container, &front); // Pops the item from the front of the Container.
		ItemType currentItem = Cells[front.row][front.col]; // Locates the popped item in the cell array.

		// If the cell is visited, skip to next iteration
		if (currentItem.visited == 1) 
		{
			continue;
		}
		// If the cell is the end cell, set found to true, change the value to *, add the index to path, and
		// break ou of the loop
		if (currentItem.value == 'e')
		{
			found = 1;
			Cells[currentItem.row][currentItem.col].value = '*';
			sprintf(buffer, "[%d,%d]", currentItem.row, currentItem.col);
			strcat(path, buffer);
			break;
		}
		// If the cell is not visited, then insert the appropriate adjacent cells into the queue,
		// set the cell to visited, change its value to *, and add its index to the path.
		if (!currentItem.visited)
		{
			InsertAdjCells(Container, currentItem, rows, cols, Cells);
			Cells[currentItem.row][currentItem.col].visited = 1;
			if (Cells[currentItem.row][currentItem.col].value == '.' || 
				Cells[currentItem.row][currentItem.col].value == 's') {
				Cells[currentItem.row][currentItem.col].value = '*';
				sprintf(buffer, "[%d,%d]", currentItem.row, currentItem.col);
				strcat(path, buffer);
			}
		}
	}
}

// Insert the appropriate adjacent cells into the Container queue.
void InsertAdjCells(Queue * Container, ItemType currentItem, int rows, int cols, ItemType Cells[rows][cols]) 
{
	// If there is enough room for a left cell, retrieve it
	if (currentItem.col > 0) 
	{
		ItemType left = Cells[currentItem.row][currentItem.col - 1];

		// If the left cell is an unoccupied, unvisited cell, then insert it
		if (!left.visited && left.value != 'o')
		{
			Insert(left, Container);
		}
	}
	// If there is enough room for a right cell, retrieve it
	if (currentItem.col < cols - 1)
	{
		ItemType right = Cells[currentItem.row][currentItem.col + 1];

		// If the right cell is an unoccupied, unvisited cell, then insert it
		if (!right.visited && right.value != 'o')
		{
			Insert(right, Container);
		}
	}
	// If there is enough room for a top cell, retrieve it
	if (currentItem.row > 0)
	{
		ItemType top = Cells[currentItem.row - 1][currentItem.col];

		// If the top cell is an unoccupied, unvisited cell, then insert it
		if (!top.visited && top.value != 'o')
		{
			Insert(top, Container);
		}
	}
	// If there is enough room for a bottom cell, retrieve it
	if (currentItem.row < rows - 1) {
		ItemType bottom = Cells[currentItem.row + 1][currentItem.col];

		// If the bottom cell is an unoccupied, unvisited cell, then insert it
		if (!bottom.visited && bottom.value != 'o')
		{
			Insert(bottom, Container);
		}
	}
}