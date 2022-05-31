#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "main.h"
#include <inttypes.h>
#include <errno.h>

// for handling malloc errors
#define DIE(assertion, call_description)            \
	do                                              \
	{                                               \
		if (assertion)                              \
		{                                           \
			fprintf(stderr, "(%s, %d): ", __FILE__, \
					__LINE__);                      \
			perror(call_description);               \
			exit(errno);                            \
		}                                           \
	} while (0)


int go_to_elem(void *arr, int len)
{
	int size = 0;
	for (int i = 0; i < len; i++)
	{
		head header = *(head *)arr;
		// increase size with no of bytes occupied by element i
		size += sizeof(head) + header.len;
		arr += sizeof(head) + header.len;
	}
	return size;
}

int add_last(void **Arr, int *len, data_structure *elem)
{
	int size = go_to_elem(*Arr, *len); // go to last position
	void *arr = *Arr;

	// realloc arr, adding the no of bytes necessary for storing 
	// the new element
	arr = realloc(arr, size + sizeof(head) + elem->header->len);
	DIE(arr == NULL, "realloc failed");

	// copy the header and the element itself
	memcpy(arr + size, elem->header, sizeof(head));
	memcpy(arr + size + sizeof(head), elem->data, elem->header->len);
	*Arr = arr;

	(*len)++; //increase length of array
	return 1;
}

int add_at(void **Arr, int *len, data_structure *elem, int idx)
{
	if (idx < 0)
		return 0;
	if (idx > *len)
		return add_last(Arr, len, elem);

	int total_size = go_to_elem(*Arr, *len); // no of bytes stored in the array
	int size_upto_idx = go_to_elem(*Arr, idx); // no of bytes stored up to the element at position idx
	int size_added = sizeof(head) + elem->header->len; // no of bytes necessary for the new element

	void *arr = *Arr;

	// realloc arr, adding the no of bytes necessary for storing 
	// the new element
	arr = realloc(arr, total_size + size_added);
	DIE(arr == NULL, "realloc failed");

	// move all the elements (starting with the one at index idx) size_added bytes
	// to the right, in order to make space for the new element
	memmove(arr + size_added + size_upto_idx, arr + size_upto_idx, total_size - size_upto_idx);

	// copy the header and the element itself
	memcpy(arr + size_upto_idx, elem->header, sizeof(head));
	memcpy(arr + size_upto_idx + sizeof(head), elem->data, elem->header->len);
	*Arr = arr;

	(*len)++; //increase length of array
	return 1;
}

int delete_at(void **Arr, int *len, int idx)
{
	if (idx < 0 || idx > *len)
		return 0;

	int total_size = go_to_elem(*Arr, *len); // no of bytes stored in the array
	int size_upto_idx = go_to_elem(*Arr, idx); // no of bytes stored up to the element at position idx
	int size_after_idx = go_to_elem(*Arr, idx + 1); // no of bytes stored up to the element at position idx + 1
	int size_deleted = size_after_idx - size_upto_idx; // no of bytes used by the element at position idx

	void *arr = *Arr;

	// move all the elements (starting with the one at index idx) size_deleted bytes
	// to the left, in order to delete the desired element
	memmove(arr + size_upto_idx, arr + size_after_idx, total_size - size_after_idx);

	// realloc the memory for the array, decreasing it with "size_deleted" bytes
	arr = realloc(arr, total_size - size_deleted);
	DIE(*len !=1 && arr == NULL, "realloc failed");
 
	*Arr = arr;

	(*len)--; //decrease the length
	return 1;
}

void print_string(void **arr)
{
	while (*(char *)(*arr) != '\0') // go until meeting string terminator
	{
		printf("%c", *(char *)(*arr)); // print char by char
		(*arr)++;
	}
}

void print_element(void *arr)
{
	head header = *(head *)arr;
	printf("Tipul %u\n", header.type); // print type

	arr = arr + sizeof(head);

	print_string(&arr); // print first name
	arr++;

	// for each type, cast the integers and print them and the second name
	if (header.type == 1)
	{
		int8_t sum1 = *(int8_t *)(arr);
		int8_t sum2 = *(int8_t *)(arr + 1);
		arr += 2;
		printf(" pentru ");
		print_string(&arr);
		printf("\n%" PRId8 "\n%" PRId8 "\n\n", sum1, sum2);
	}
	else if (header.type == 2)
	{
		int16_t sum1 = *(int16_t *)(arr);
		int32_t sum2 = *(int32_t *)(arr + 2);
		arr += 6;
		printf(" pentru ");
		print_string(&arr);
		printf("\n%" PRId16 "\n%" PRId32 "\n\n", sum1, sum2);
	}
	else if (header.type == 3)
	{
		int32_t sum1 = *(int32_t *)(arr);
		int32_t sum2 = *(int32_t *)(arr + 4);
		arr += 8;
		printf(" pentru ");
		print_string(&arr);
		printf("\n%" PRId32 "\n%" PRId32 "\n\n", sum1, sum2);
	}
}

void find(void *arr, int len, int idx)
{
	if (idx < 0 || idx > len)
		return;

	int size = go_to_elem(arr, idx);
	print_element(arr + size);
}

void print(void *arr, int len)
{
	for (int idx = 0; idx < len; idx++)
	{
		int size = go_to_elem(arr, idx);
		print_element(arr + size);
	}
}

data_structure *read_input(char *p)
{	
	p = strtok(NULL, " ");
	int type = atoi(p);

	// alloc space for the data_structure that stores the new element
	data_structure *elem = malloc(sizeof(data_structure));
	DIE(elem == NULL, "malloc failed");

	// alloc space for the header
	elem->header = malloc(sizeof(head));
	DIE(elem->header == NULL, "malloc failed");

	elem->header->type = type; // assign the input type

	p = strtok(NULL, " ");					// first name
	elem->header->len = strlen(p) + 1;		// modify no of bytes stores in data
	elem->data = malloc(elem->header->len); // alloc memory for storing first name
	DIE(elem->data == NULL, "malloc failed");
	memcpy(elem->data, p, strlen(p) + 1); // copy first name in data

	p = strtok(NULL, " ");

	// based on the desired type, cast the input integers, count their size in bytes,
	// realloc the data field and the integer values to it
	if (type == 1)
	{
		int8_t sum1 = atoi(p);
		p = strtok(NULL, " ");
		int8_t sum2 = atoi(p);
		int size_added = sizeof(sum1) + sizeof(sum2);

		elem->data = realloc(elem->data, elem->header->len + size_added);
		DIE(elem->data == NULL, "realloc failed");

		memcpy((elem->data) + elem->header->len, &sum1, sizeof(sum1));
		memcpy((elem->data) + elem->header->len + sizeof(sum1), &sum2, sizeof(sum2));

		elem->header->len += size_added; // modify no of bytes stores in data
	}
	else if (type == 2)
	{
		int16_t sum1 = atoi(p);
		p = strtok(NULL, " ");
		int32_t sum2 = atoi(p);
		int size_added = sizeof(sum1) + sizeof(sum2);

		elem->data = realloc(elem->data, elem->header->len + size_added);
		DIE(elem->data == NULL, "realloc failed");

		memcpy((elem->data) + elem->header->len, &sum1, sizeof(sum1));
		memcpy((elem->data) + elem->header->len + sizeof(sum1), &sum2, sizeof(sum2));

		elem->header->len += size_added;
	}
	else if (type == 3)
	{
		int32_t sum1 = atoi(p);
		p = strtok(NULL, " ");
		int32_t sum2 = atoi(p);
		int size_added = sizeof(sum1) + sizeof(sum2);

		elem->data = realloc(elem->data, elem->header->len + size_added);
		DIE(elem->data == NULL, "realloc failed");

		memcpy((elem->data) + elem->header->len, &sum1, sizeof(sum1));
		memcpy((elem->data) + elem->header->len + sizeof(sum1), &sum2, sizeof(sum2));

		elem->header->len += size_added;
	}

	p = strtok(NULL, " "); // second name
	 // realloc memory to store second name in the data feild
	elem->data = realloc(elem->data, elem->header->len + strlen(p) + 1);
	DIE(elem->data == NULL, "realloc failed");

	memcpy(elem->data + elem->header->len, p, strlen(p));
	elem->header->len += strlen(p) + 1; // copy second name in data

	return elem;
}

int main()
{
	int len = 0;
	void *arr = NULL;

	char command[256];
	while (fgets(command, 256, stdin))
	{
		if (command[strlen(command) - 1] == '\n') // delete trailing newline
			command[strlen(command) - 1] = '\0';

		char *p = strtok(command, " "); // p = command

		if (!strcmp(p, "insert"))
		{
			data_structure *elem = read_input(p);
			add_last(&arr, &len, elem);

			free(elem->data);
			free(elem->header);
			free(elem);
		}
		else if (!strcmp(p, "insert_at"))
		{
			p = strtok(NULL, " ");
			int idx = atoi(p);
			data_structure *elem = read_input(p);
			add_at(&arr, &len, elem, idx);

			free(elem->data);
			free(elem->header);
			free(elem);
		}
		else if (!strcmp(p, "delete_at"))
		{
			p = strtok(NULL, " ");
			int idx = atoi(p);
			delete_at(&arr, &len, idx);
		}
		else if (!strcmp(p, "find"))
		{
			p = strtok(NULL, " ");
			int idx = atoi(p);
			find(arr, len, idx);
		}
		else if (!strcmp(p, "print"))
		{
			print(arr, len);
		}
		else if (!strcmp(p, "exit"))
		{
			free(arr);
		}
	}

	return 0;
}
