#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// goes to the starting position of the len + 1 element and
// returns the no of bytes occupied by the first len elements
int go_to_elem(void *arr, int len);

// copies the information from a data_structure element
// to the end of the array
int add_last(void **Arr, int *len, data_structure *elem);

// copies the information from a data_structure element
// to a desired position in the array
int add_at(void **Arr, int *len, data_structure *elem, int idx);

// deletes the information of a certain element in the array
int delete_at(void **Arr, int *len, int idx);

// prints characters until finding \0
void print_string(void **arr);

// finds the element at position idx in the array and prints it
void find(void *arr, int len, int idx);

// prints all elements in the array
void print(void *arr, int len);

// reads input from stdin and stores it inside
// a "data_structure" type of element
data_structure *read_input(char *p);
