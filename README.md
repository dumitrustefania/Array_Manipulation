**Copyright - Dumitru Bianca Stefania - 312CA - 2022**

## Homework #1 - PCLP2

## General description and notes:

- The program's main purpose is storing and opertaing with an array of organised data. 
- Each element of the array represents a contigous slice of memory that contains a name, 2 integers and a second name. 
- The integers can be part of 1 out of 3 categories:
    1 - int8_t & int8_t;
    2 - int16_t & int16_t;
    3 - int32_t & int32_t,
        where the B in intB_t represents the number of bits needed for storing the integer.
- The names are basic arrays of characters, that are delimitated by the string terminator, '\0'.
- Apart from the data itself, the array also stores a header for each element, that indicates the type of 
    integers and the number of bytes used for storing each element.
- All the memory is dinamically allocated, using alloc, realloc and free functions from the stdlib library.
- The information for each element is received from stdin and is initially stored inside a variable of type "data_structure".
    The header is updated with the relevant information and the names and integers are copied inside the data field of the variable
    in order to be later stored inside the array.

## Commands:
- There are 6 possible commands that can be recieved from input:

1) add_last
    - reallocates the array, making space for the new element, stored initially as a "data_structure" variable
    - copies the information from the variable to the end of the array

2) add_at <idx>
    - reallocates the array, making space for the new element, stored initially as a "data_structure" variable
    - moves all the elements starting with index idx one position to the right, in order to fit the new element in the created space
    - copies the information from the variable into the array at position idx

3) find <idx>
    - finds the element at position idx inside the array and prints its contents

4) delete_at <idx>
    - moves all the elements starting with index idx one position to the left, in order to erase the element that used to be at position idx
    - reallocates the array, eliminating memory corresponding to the deleted element

5) print
    - prints all elements stored in the array

6) exit
    - frees the memory for the array
    - exits the program
