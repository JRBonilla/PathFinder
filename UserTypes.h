#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Defines an item with a row, column, value, and a
// visited value (0 for not visited, 1 for visited).
typedef struct {
	int row, col;
	char value;
	bool visited;
} ItemType;