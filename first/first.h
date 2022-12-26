/*
General Purpose Functions
Strictly Not Customizable: Do Not Change GPF Functionalities.
Do Not Create any Unused or Unnecessary functionalities that hinder the Software Design Principles.
*/

/*
Documentation is given below for all GPF functionalities.
*/

/*
Memory Management Setup:
Place this declaration at the top of your program's file: 
	memory_cdll *memory;
Within your program's main function, place this at the beginning: 
	memory = memory_cdll_constructor();
After each call to malloc(), place this: 
	memory_cdll_push(memory, pointer);
When you want to free the memory: 
	In the middle of your program: 
		memory_cdll_free(memory, true);
	At the end of your program: 
		memory_cdll_free(memory, false);
If you want to store temporary memory for a target function, consider creating a new memory_cdll object:
	Within your target function, place this at the beginning:
		memory_cdll *tmp_memory = memory_cdll_constructor();
	Continue to use the push function: 
		memory_cdll_push(tmp_memory, pointer);
	Free the tmp_memory as needed in the middle or at the end of your target function.
	You should create a new memory_cdll object for each target process or algorithm.
If you need to free a pointer manually in your program:
	pointer = Free(pointer);
	You can re-use the same pointer again, if you want, because it is set to NULL after it is freed.
*/

/*
readLine_char_firstD* readLine(FILE *fp, readLine_char_firstD *line, unsigned int count_chars_in_delimiter, char *delimiter):
	Reads line by line from a file.
	Returns a line, split into parts using the delimiter.
	(*line).i[index1].j = Gives a char *j string at index1.
	(*line).i[index1].j[index2] = Gives a char at (index1, index2).
	(*line).size = Gives the size of the i-array, which is an array of structs that each contain a char *j string.
	(*line).i[index1].size = Gives the size of the char *j string contained in the struct at index1.
	Visual:
	The Left side is an array of structs i[{index1}, {index2}, {index3},] going downwards, and its size (First Dimension).
	The Right side is an array of characters, also known as a string, and its size (Second Dimension).
	i[
	{index1}, -> j"String 1\0", size = 9
	{index2}, -> j"String 2\0", size = 9
	{index3}, -> j"String 3\0", size = 9
	], size = 3
	Use:
	readLine_char_firstD *line = NULL;
	while ((line = readLine(fp, line, 3, ",- ")) != NULL) {
		if ((*line).size == 0) {
			// What to do if it is a blank line. By default, it is skipped.
			continue;
		}
		// Process the line here
	}
	line = readLine_free(line);
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void* Free(void *pointer) {
	free(pointer);
	pointer = NULL;
	return NULL;
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct memory_cdllnode;
struct memory_cdll;

typedef struct memory_cdllnode {
	struct memory_cdllnode *next;
	struct memory_cdllnode *prev;
	void *pointer;
} memory_cdllnode;

typedef struct memory_cdll {
	struct memory_cdllnode *tail;
	unsigned int num_of_nodes;
} memory_cdll;

memory_cdllnode* memory_cdllnode_constructor(void);
memory_cdll* memory_cdll_constructor(void);
memory_cdllnode* memory_cdllnode_getNext(memory_cdllnode *self);
memory_cdllnode* memory_cdllnode_getPrev(memory_cdllnode *self);
void memory_cdllnode_setNext(memory_cdllnode *self, memory_cdllnode *next);
void memory_cdllnode_setPrev(memory_cdllnode *self, memory_cdllnode *prev);
bool memory_cdll_isEmpty(memory_cdll *self);
memory_cdllnode* memory_cdllnode_free(memory_cdllnode *self);
memory_cdll* memory_cdll_free(memory_cdll *self, bool onlyNodes);
void memory_cdll_toString(memory_cdll *self);
void memory_cdll_addToHead(memory_cdll *self, void *pointer);
void memory_cdll_addToTail(memory_cdll *self, void *pointer);
memory_cdllnode* memory_cdll_removeHead(memory_cdll *self, bool returnHead);
memory_cdllnode* memory_cdll_removeTail(memory_cdll *self, bool returnTail);
void (*memory_cdll_enqueue)(memory_cdll *, void*) = memory_cdll_addToTail;
memory_cdllnode* (*memory_cdll_dequeue)(memory_cdll *, bool) = memory_cdll_removeHead;
void (*memory_cdll_push)(memory_cdll *, void*) = memory_cdll_addToHead;
memory_cdllnode* (*memory_cdll_pop)(memory_cdll *, bool) = memory_cdll_removeHead;

memory_cdllnode* memory_cdllnode_constructor(void) {
	memory_cdllnode *self = malloc(sizeof(memory_cdllnode));
	(*self).next = NULL;
	(*self).prev = NULL;
	(*self).pointer = NULL;
	return self;
}

memory_cdll* memory_cdll_constructor(void) {
	memory_cdll *self = malloc(sizeof(memory_cdll));
	(*self).tail = NULL;
	(*self).num_of_nodes = 0;
	return self;
}

memory_cdllnode* memory_cdllnode_getNext(memory_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	return (*self).next;
}

memory_cdllnode* memory_cdllnode_getPrev(memory_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	return (*self).prev;
}

void memory_cdllnode_setNext(memory_cdllnode *self, memory_cdllnode *next) {
	if (self == NULL) {
		return;
	}
	(*self).next = next;
	return;
}

void memory_cdllnode_setPrev(memory_cdllnode *self, memory_cdllnode *prev) {
	if (self == NULL) {
		return;
	}
	(*self).prev = prev;
	return;
}

bool memory_cdll_isEmpty(memory_cdll *self) {
	if (self == NULL) {
		return true;
	}
	return ((*self).num_of_nodes == 0);
}

memory_cdllnode* memory_cdllnode_free(memory_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	(*self).pointer = Free((*self).pointer);
	self = Free(self);
	return NULL;
}

memory_cdll* memory_cdll_free(memory_cdll *self, bool onlyNodes) {
	if (self == NULL) {
		return NULL;
	}
	unsigned int num_of_nodes = (*self).num_of_nodes;
	for (unsigned int i = 0; i < num_of_nodes; i++) {
		memory_cdll_removeHead(self, false);
	}
	if (onlyNodes == false) {
		self = Free(self);
		return NULL;
	}
	return self;
}

void memory_cdll_toString(memory_cdll *self) {
	if (self == NULL || memory_cdll_isEmpty(self)) {
		printf("EMPTY\n");
		return;
	}
	unsigned int num_of_nodes = (*self).num_of_nodes;
	memory_cdllnode *head = memory_cdllnode_getNext((*self).tail);
	if (num_of_nodes == 1) {
		printf("%p\n", (*head).pointer);
		return;
	}
	for (unsigned int i = 0; i < num_of_nodes - 1; i++) {
		printf("%p ", (*head).pointer);
		head = memory_cdllnode_getNext(head);
	}
	printf("%p\n", (*head).pointer);
	return;
}

void memory_cdll_addToHead(memory_cdll *self, void *pointer) {
	if (self == NULL) {
		return;
	}
	memory_cdllnode *newnode = memory_cdllnode_constructor();
	(*newnode).pointer = pointer;
	if (memory_cdll_isEmpty(self)) {
		(*self).tail = newnode;
		(*self).tail->next = newnode;
		(*self).tail->prev = newnode;
		(*self).num_of_nodes++;
		return;
	}
	memory_cdllnode_setNext(newnode, memory_cdllnode_getNext((*self).tail));
	(*self).tail->next = newnode;
	(*newnode).next->prev = newnode;
	(*newnode).prev = (*self).tail;
	(*self).num_of_nodes++;
	return;
}

void memory_cdll_addToTail(memory_cdll *self, void *pointer) {
	if (self == NULL) {
		return;
	}
	memory_cdllnode *newnode = memory_cdllnode_constructor();
	(*newnode).pointer = pointer;
	if (memory_cdll_isEmpty(self)) {
		(*self).tail = newnode;
		(*self).tail->next = newnode;
		(*self).tail->prev = newnode;
		(*self).num_of_nodes++;
		return;
	}
	memory_cdllnode_setNext(newnode, memory_cdllnode_getNext((*self).tail));
	memory_cdllnode_setNext((*self).tail, newnode);
	(*newnode).prev = (*self).tail;
	(*self).tail = newnode;
	(*self).tail->next->prev = (*self).tail;
	(*self).num_of_nodes++;
	return;
}

memory_cdllnode* memory_cdll_removeHead(memory_cdll *self, bool returnHead) {
	if (self == NULL || memory_cdll_isEmpty(self)) {
		return NULL;
	}
	memory_cdllnode *head = memory_cdllnode_getNext((*self).tail);
	(*self).tail->next = (*head).next;
	(*self).tail->next->prev = (*self).tail;
	(*head).next = NULL;
	(*head).prev = NULL;
	(*self).num_of_nodes--;
	if (memory_cdll_isEmpty(self)) {
		(*self).tail = NULL;
	}
	if (returnHead == true) {
		return head;
	}
	head = memory_cdllnode_free(head);
	return NULL;
}

memory_cdllnode* memory_cdll_removeTail(memory_cdll *self, bool returnTail) {
	if (self == NULL || memory_cdll_isEmpty(self)) {
		return NULL;
	}
	memory_cdllnode *tail = (*self).tail;
	memory_cdllnode *prev = (*self).tail->prev;
	(*prev).next = (*tail).next;
	(*tail).next->prev = prev;
	(*self).tail = prev;
	(*tail).next = NULL;
	(*tail).prev = NULL;
	(*self).num_of_nodes--;
	if (memory_cdll_isEmpty(self)) {
		(*self).tail = NULL;
	}
	if (returnTail == true) {
		return tail;
	}
	tail = memory_cdllnode_free(tail);
	return NULL;
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct readLine_cdllnode;
struct readLine_cdll;

typedef struct readLine_cdllnode {
	struct readLine_cdllnode *next;
	struct readLine_cdllnode *prev;
	char c;
} readLine_cdllnode;

typedef struct readLine_cdll {
	struct readLine_cdllnode *tail;
	unsigned int num_of_nodes;
} readLine_cdll;

readLine_cdllnode* readLine_cdllnode_constructor(void);
readLine_cdll* readLine_cdll_constructor(void);
readLine_cdllnode* readLine_cdllnode_getNext(readLine_cdllnode *self);
readLine_cdllnode* readLine_cdllnode_getPrev(readLine_cdllnode *self);
void readLine_cdllnode_setNext(readLine_cdllnode *self, readLine_cdllnode *next);
void readLine_cdllnode_setPrev(readLine_cdllnode *self, readLine_cdllnode *prev);
bool readLine_cdll_isEmpty(readLine_cdll *self);
readLine_cdllnode* readLine_cdllnode_free(readLine_cdllnode *self);
readLine_cdll* readLine_cdll_free(readLine_cdll *self, bool onlyNodes);
void readLine_cdll_toString(readLine_cdll *self);
void readLine_cdll_addToHead(readLine_cdll *self, char c);
void readLine_cdll_addToTail(readLine_cdll *self, char c);
readLine_cdllnode* readLine_cdll_removeHead(readLine_cdll *self, bool returnHead);
readLine_cdllnode* readLine_cdll_removeTail(readLine_cdll *self, bool returnTail);
void (*readLine_cdll_enqueue)(readLine_cdll *, char) = readLine_cdll_addToTail;
readLine_cdllnode* (*readLine_cdll_dequeue)(readLine_cdll *, bool) = readLine_cdll_removeHead;
void (*readLine_cdll_push)(readLine_cdll *, char) = readLine_cdll_addToHead;
readLine_cdllnode* (*readLine_cdll_pop)(readLine_cdll *, bool) = readLine_cdll_removeHead;

readLine_cdllnode* readLine_cdllnode_constructor(void) {
	readLine_cdllnode *self = malloc(sizeof(readLine_cdllnode));
	(*self).next = NULL;
	(*self).prev = NULL;
	(*self).c = '\0';
	return self;
}

readLine_cdll* readLine_cdll_constructor(void) {
	readLine_cdll *self = malloc(sizeof(readLine_cdll));
	(*self).tail = NULL;
	(*self).num_of_nodes = 0;
	return self;
}

readLine_cdllnode* readLine_cdllnode_getNext(readLine_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	return (*self).next;
}

readLine_cdllnode* readLine_cdllnode_getPrev(readLine_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	return (*self).prev;
}

void readLine_cdllnode_setNext(readLine_cdllnode *self, readLine_cdllnode *next) {
	if (self == NULL) {
		return;
	}
	(*self).next = next;
	return;
}

void readLine_cdllnode_setPrev(readLine_cdllnode *self, readLine_cdllnode *prev) {
	if (self == NULL) {
		return;
	}
	(*self).prev = prev;
	return;
}

bool readLine_cdll_isEmpty(readLine_cdll *self) {
	if (self == NULL) {
		return true;
	}
	return ((*self).num_of_nodes == 0);
}

readLine_cdllnode* readLine_cdllnode_free(readLine_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	self = Free(self);
	return NULL;
}

readLine_cdll* readLine_cdll_free(readLine_cdll *self, bool onlyNodes) {
	if (self == NULL) {
		return NULL;
	}
	unsigned int num_of_nodes = (*self).num_of_nodes;
	for (unsigned int i = 0; i < num_of_nodes; i++) {
		readLine_cdll_removeHead(self, false);
	}
	if (onlyNodes == false) {
		self = Free(self);
		return NULL;
	}
	return self;
}

void readLine_cdll_toString(readLine_cdll *self) {
	if (self == NULL || readLine_cdll_isEmpty(self)) {
		printf("EMPTY\n");
		return;
	}
	unsigned int num_of_nodes = (*self).num_of_nodes;
	readLine_cdllnode *head = readLine_cdllnode_getNext((*self).tail);
	if (num_of_nodes == 1) {
		printf("%c\n", (*head).c);
		return;
	}
	for (unsigned int i = 0; i < num_of_nodes - 1; i++) {
		printf("%c ", (*head).c);
		head = readLine_cdllnode_getNext(head);
	}
	printf("%c\n", (*head).c);
	return;
}

void readLine_cdll_addToHead(readLine_cdll *self, char c) {
	if (self == NULL) {
		return;
	}
	readLine_cdllnode *newnode = readLine_cdllnode_constructor();
	(*newnode).c = c;
	if (readLine_cdll_isEmpty(self)) {
		(*self).tail = newnode;
		(*self).tail->next = newnode;
		(*self).tail->prev = newnode;
		(*self).num_of_nodes++;
		return;
	}
	readLine_cdllnode_setNext(newnode, readLine_cdllnode_getNext((*self).tail));
	(*self).tail->next = newnode;
	(*newnode).next->prev = newnode;
	(*newnode).prev = (*self).tail;
	(*self).num_of_nodes++;
	return;
}

void readLine_cdll_addToTail(readLine_cdll *self, char c) {
	if (self == NULL) {
		return;
	}
	readLine_cdllnode *newnode = readLine_cdllnode_constructor();
	(*newnode).c = c;
	if (readLine_cdll_isEmpty(self)) {
		(*self).tail = newnode;
		(*self).tail->next = newnode;
		(*self).tail->prev = newnode;
		(*self).num_of_nodes++;
		return;
	}
	readLine_cdllnode_setNext(newnode, readLine_cdllnode_getNext((*self).tail));
	readLine_cdllnode_setNext((*self).tail, newnode);
	(*newnode).prev = (*self).tail;
	(*self).tail = newnode;
	(*self).tail->next->prev = (*self).tail;
	(*self).num_of_nodes++;
	return;
}

readLine_cdllnode* readLine_cdll_removeHead(readLine_cdll *self, bool returnHead) {
	if (self == NULL || readLine_cdll_isEmpty(self)) {
		return NULL;
	}
	readLine_cdllnode *head = readLine_cdllnode_getNext((*self).tail);
	(*self).tail->next = (*head).next;
	(*self).tail->next->prev = (*self).tail;
	(*head).next = NULL;
	(*head).prev = NULL;
	(*self).num_of_nodes--;
	if (readLine_cdll_isEmpty(self)) {
		(*self).tail = NULL;
	}
	if (returnHead == true) {
		return head;
	}
	head = readLine_cdllnode_free(head);
	return NULL;
}

readLine_cdllnode* readLine_cdll_removeTail(readLine_cdll *self, bool returnTail) {
	if (self == NULL || readLine_cdll_isEmpty(self)) {
		return NULL;
	}
	readLine_cdllnode *tail = (*self).tail;
	readLine_cdllnode *prev = (*self).tail->prev;
	(*prev).next = (*tail).next;
	(*tail).next->prev = prev;
	(*self).tail = prev;
	(*tail).next = NULL;
	(*tail).prev = NULL;
	(*self).num_of_nodes--;
	if (readLine_cdll_isEmpty(self)) {
		(*self).tail = NULL;
	}
	if (returnTail == true) {
		return tail;
	}
	tail = readLine_cdllnode_free(tail);
	return NULL;
}

typedef struct readLine_char_secondD {
	char *j;
	unsigned int size;
} readLine_char_secondD;

typedef struct readLine_char_firstD {
	struct readLine_char_secondD *i;
	unsigned int size;
} readLine_char_firstD;

bool readLine_isDelimiter(char c, unsigned int count_chars_in_delimiter, char *delimiter) {
	for (int i = 0; i < count_chars_in_delimiter; i++) {
		if (delimiter[i] == c) {
			return true;
		}
	}
	return false;
}

readLine_char_firstD* readLine_free(readLine_char_firstD *line) {
	if (line != NULL) {
		unsigned int firstDSize = (*line).size;
		for (unsigned int i = 0; i < firstDSize; i++) {
			(*line).i[i].j = Free((*line).i[i].j);
		}
		(*line).i = Free((*line).i);
    	line = Free(line);
	}
	return line;
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

readLine_char_firstD* readLine(FILE *fp, readLine_char_firstD *line, unsigned int count_chars_in_delimiter, char *delimiter) {
	line = readLine_free(line);
    int c;
    readLine_cdll *cdll = readLine_cdll_constructor();
    while (true) {
        c = fgetc(fp);
		if (feof(fp) && readLine_cdll_isEmpty(cdll)) {
			cdll = readLine_cdll_free(cdll, false);
			return line;
		}
        if (c == '\n' && readLine_cdll_isEmpty(cdll)) {
            line = malloc(sizeof(readLine_char_firstD));
            (*line).i = NULL;
			(*line).size = 0;
			cdll = readLine_cdll_free(cdll, false);
            return line;
        }
        if (c == '\n' || feof(fp)) {
            unsigned int num_of_nodes = (*cdll).num_of_nodes;
			readLine_cdllnode *head = (*cdll).tail->next;
			unsigned int num_of_compartments = 1;
            for (unsigned int i = 0; i < num_of_nodes; i++) {
				if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) && i == 0) {
					for (i = 1; i < num_of_nodes; i++) {
						head = readLine_cdllnode_getNext(head);
						if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false) {
							break;
						}
					}
					if (i == num_of_nodes) {
						line = malloc(sizeof(readLine_char_firstD));
            			(*line).i = NULL;
						(*line).size = 0;
						cdll = readLine_cdll_free(cdll, false);
            			return line;
					}
				}
				if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter)) {
					if (readLine_isDelimiter((*head).prev->c, count_chars_in_delimiter, delimiter) == false) {
						for (i = i + 1; i < num_of_nodes; i++) {
							head = readLine_cdllnode_getNext(head);
							if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false) {
								num_of_compartments++;
								break;
							}
						}
					}
				}
				head = readLine_cdllnode_getNext(head);
			}
			head = (*cdll).tail->next;
			line = malloc(sizeof(readLine_char_firstD));
			(*line).i = malloc(sizeof(readLine_char_secondD) * num_of_compartments);
			(*line).size = num_of_compartments;
			readLine_cdll *comp = readLine_cdll_constructor();
			unsigned int compartment = 0;
			for (unsigned int i = 0; i < num_of_nodes; i++) {
				if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) && i == 0) {
					for (i = 1; i < num_of_nodes; i++) {
						head = readLine_cdllnode_getNext(head);
						if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false) {
							break;
						}
					}
				}
				for (i = i; i < num_of_nodes; i++) {
					if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false && i == num_of_nodes - 1) {
						readLine_cdll_addToTail(comp, (*head).c);
						unsigned int compSize = (*comp).num_of_nodes;
						(*line).i[compartment].j = malloc(sizeof(char) * (compSize + 1));
						(*line).i[compartment].size = compSize + 1;
						(*line).i[compartment].j[compSize] = '\0';
						readLine_cdllnode *cmphead = (*comp).tail->next;
						for (int i = 0; i < compSize; i++) {
							(*line).i[compartment].j[i] = (*cmphead).c;
							cmphead = readLine_cdllnode_getNext(cmphead);
						}
						comp = readLine_cdll_free(comp, false);
						cdll = readLine_cdll_free(cdll, false);
						return line;
					}
					if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false) {
						readLine_cdll_addToTail(comp, (*head).c);
						head = readLine_cdllnode_getNext(head);
						continue;
					}
					unsigned int compSize = (*comp).num_of_nodes;
					(*line).i[compartment].j = malloc(sizeof(char) * (compSize + 1));
					(*line).i[compartment].size = compSize + 1;
					(*line).i[compartment].j[compSize] = '\0';
					readLine_cdllnode *cmphead = (*comp).tail->next;
					for (int i = 0; i < compSize; i++) {
						(*line).i[compartment].j[i] = (*cmphead).c;
						cmphead = readLine_cdllnode_getNext(cmphead);
					}
					compartment++;
					comp = readLine_cdll_free(comp, true);
					break;
				}
				if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter)) {
					if (readLine_isDelimiter((*head).prev->c, count_chars_in_delimiter, delimiter) == false) {
						for (i = i + 1; i < num_of_nodes; i++) {
							head = readLine_cdllnode_getNext(head);
							if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false) {
								i--;
								head = readLine_cdllnode_getPrev(head);
								break;
							}
						}
						if (i == num_of_nodes) {
							comp = readLine_cdll_free(comp, false);
							cdll = readLine_cdll_free(cdll, false);
							return line;
						}
					}
				}
				head = readLine_cdllnode_getNext(head);
			}
			comp = readLine_cdll_free(comp, false);
            cdll = readLine_cdll_free(cdll, false);
            return line;
        }
        readLine_cdll_addToTail(cdll, c);
    }
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

typedef enum gate_type {
	GATE_COUNT = 6,
	OR,
	AND,
	XOR,
	NOT,
	DECODER,
	MULTIPLEXER,
	OTHER
} gate_type;

struct input_output;
struct gate;
struct circuit;
struct input;
struct input_value;
struct output;
struct output_value;
struct outcomes;

typedef struct input_output {
	char **inputs;
	char **outputs;
	char **selectors;
	size_t num_of_inputs;
	size_t num_of_outputs;
	size_t num_of_selectors;
} input_output;

typedef struct gate {
	gate_type type;
	input_output *input_output;
} gate;

typedef struct circuit {
	gate **gates;
	size_t *other_gate_indices;
	size_t *other_gate_permutations;
	size_t num_of_gates;
	size_t num_of_other_gates;
	size_t num_of_other_gate_permutations;
} circuit;

typedef struct input {
	char *input;
	size_t value;
} input;

typedef struct input_value {
	input **inputs;
	size_t original_num_of_inputs;
	size_t current_num_of_inputs;
	size_t max_num_of_inputs;
} input_value;

typedef struct output {
	char *output;
	size_t *values;
	size_t num_of_values;
} output;

typedef struct output_value {
	output **outputs;
	output **gen_outputs;
	size_t num_of_outputs;
} output_value;

typedef struct outcomes {
	size_t *outcomes;
	size_t num_of_outcomes;
} outcomes;

char** copy_array_of_strings(input **source, size_t current_num_of_strings, size_t max_num_of_strings) {
	char **copy = malloc(sizeof(char*) * max_num_of_strings);
	for (size_t i = 0; i < max_num_of_strings; i++) {
		if (i < current_num_of_strings) {
			copy[i] = malloc(sizeof(char) * (strlen(source[i]->input) + 1));
			strcpy(copy[i], source[i]->input);
		} else {
			copy[i] = NULL;
		}
	}
	return copy;
}

void add_string_to_array(char *str, char **arr, size_t current_num_of_strings) {
	arr[current_num_of_strings] = malloc(sizeof(char) * (strlen(str) + 1));
	strcpy(arr[current_num_of_strings], str);
	return;
}

char** free_array_of_strings(char **arr, size_t num_of_strings) {
	for (size_t i = 0; i < num_of_strings; i++) {
		arr[i] = Free(arr[i]);
	}
	arr = Free(arr);
	return NULL;
}

bool array_contains_string(char *str, char **arr, size_t num_of_strings) {
	for (size_t i = 0; i < num_of_strings; i++) {
		if (strcmp(arr[i], str) == 0) {
			return true;
		}
	}
	return false;
}

size_t logarithm_base_2_floored(size_t number) {
	size_t result = 0;
	size_t ONE = 1;
	if (number == 0) {
		return result;
	}
	while ((ONE << result) <= number) {
		result++;
	}
	return result - ONE;
}

size_t pows(size_t base, size_t exponent) {
	if (exponent == 0) {
		return 1;
	}
	size_t product = 1;
	for (size_t i = 0; i < exponent; i++) {
		product *= base;
	}
	return product;
}

input_output* input_output_constructor() {
	input_output *new_input_output = malloc(sizeof(input_output));
	new_input_output->inputs = NULL;
	new_input_output->outputs = NULL;
	new_input_output->selectors = NULL;
	new_input_output->num_of_inputs = 0;
	new_input_output->num_of_outputs = 0;
	new_input_output->num_of_selectors = 0;
	return new_input_output;
}

gate* gate_constructor() {
	gate *new_gate = malloc(sizeof(gate));
	new_gate->type = OR;
	new_gate->input_output = input_output_constructor();
	return new_gate;
}

circuit* circuit_constructor() {
	circuit *new_circuit = malloc(sizeof(circuit));
	new_circuit->gates = NULL;
	new_circuit->other_gate_indices = NULL;
	new_circuit->other_gate_permutations = NULL;
	new_circuit->num_of_gates = 0;
	new_circuit->num_of_other_gates = 0;
	new_circuit->num_of_other_gate_permutations = 0;
	return new_circuit;
}

input* input_constructor() {
	input *new_input = malloc(sizeof(input));
	new_input->input = NULL;
	new_input->value = 0;
	return new_input;
}

input_value* input_value_constructor() {
	input_value *new_input_value = malloc(sizeof(input_value));
	new_input_value->inputs = NULL;
	new_input_value->original_num_of_inputs = 0;
	new_input_value->current_num_of_inputs = 0;
	new_input_value->max_num_of_inputs = 0;
	return new_input_value;
}

output* output_constructor() {
	output *new_output = malloc(sizeof(output));
	new_output->output = NULL;
	new_output->values = NULL;
	new_output->num_of_values = 0;
	return new_output;
}

output_value* output_value_constructor() {
	output_value *new_output_value = malloc(sizeof(output_value));
	new_output_value->outputs = NULL;
	new_output_value->gen_outputs = NULL;
	new_output_value->num_of_outputs = 0;
	return new_output_value;
}

outcomes* outcomes_constructor() {
	outcomes *new_outcomes = malloc(sizeof(outcomes));
	new_outcomes->outcomes = NULL;
	new_outcomes->num_of_outcomes = 0;
	return new_outcomes;
}

input_output* input_output_free(input_output *input_output, bool keep_outer) {
	if (input_output == NULL) {
		return NULL;
	}
	size_t num_of_inputs = input_output->num_of_inputs;
	size_t num_of_outputs = input_output->num_of_outputs;
	size_t num_of_selectors = input_output->num_of_selectors;
	for (size_t i = 0; i < num_of_inputs; i++) {
		input_output->inputs[i] = Free(input_output->inputs[i]);
	}
	input_output->inputs = Free(input_output->inputs);
	for (size_t i = 0; i < num_of_outputs; i++) {
		input_output->outputs[i] = Free(input_output->outputs[i]);
	}
	input_output->outputs = Free(input_output->outputs);
	for (size_t i = 0; i < num_of_selectors; i++) {
		input_output->selectors[i] = Free(input_output->selectors[i]);
	}
	input_output->selectors = Free(input_output->selectors);
	if (!keep_outer) {
		input_output = Free(input_output);
		return NULL;
	}
	input_output->num_of_inputs = 0;
	input_output->num_of_outputs = 0;
	input_output->num_of_selectors = 0;
	return NULL;
}

gate* gate_free(gate *gate, bool keep_outer) {
	if (gate == NULL) {
		return NULL;
	}
	gate->input_output = input_output_free(gate->input_output, false);
	if (!keep_outer) {
		gate = Free(gate);
		return NULL;
	}
	gate->type = OR;
	gate->input_output = input_output_constructor();
	return NULL;
}

circuit* circuit_free(circuit *circuit, bool keep_outer) {
	if (circuit == NULL) {
		return NULL;
	}
	size_t num_of_gates = circuit->num_of_gates;
	for (size_t i = 0; i < num_of_gates; i++) {
		circuit->gates[i] = gate_free(circuit->gates[i], false);
	}
	circuit->gates = Free(circuit->gates);
	circuit->other_gate_indices = Free(circuit->other_gate_indices);
	circuit->other_gate_permutations = Free(circuit->other_gate_permutations);
	if (!keep_outer) {
		circuit = Free(circuit);
		return NULL;
	}
	circuit->num_of_gates = 0;
	circuit->num_of_other_gates = 0;
	circuit->num_of_other_gate_permutations = 0;
	return NULL;
}

input* input_free(input *input, bool keep_outer) {
	if (input == NULL) {
		return NULL;
	}
	input->input = Free(input->input);
	if (!keep_outer) {
		input = Free(input);
		return NULL;
	}
	input->value = 0;
	return NULL;
}

input_value* input_value_free(input_value *input_value, bool keep_outer) {
	if (input_value == NULL) {
		return NULL;
	}
	size_t num_of_inputs = input_value->max_num_of_inputs;
	for (size_t i = 0; i < num_of_inputs; i++) {
		input_value->inputs[i] = input_free(input_value->inputs[i], false);
	}
	input_value->inputs = Free(input_value->inputs);
	if (!keep_outer) {
		input_value = Free(input_value);
		return NULL;
	}
	input_value->original_num_of_inputs = 0;
	input_value->current_num_of_inputs = 0;
	input_value->max_num_of_inputs = 0;
	return NULL;
}

output* output_free(output *output, bool keep_outer) {
	if (output == NULL) {
		return NULL;
	}
	output->output = Free(output->output);
	output->values = Free(output->values);
	if (!keep_outer) {
		output = Free(output);
		return NULL;
	}
	output->num_of_values = 0;
	return NULL;
}

output_value* output_value_free(output_value *output_value, bool keep_outer) {
	if (output_value == NULL) {
		return NULL;
	}
	size_t num_of_outputs = output_value->num_of_outputs;
	for (size_t i = 0; i < num_of_outputs; i++) {
		output_value->outputs[i] = output_free(output_value->outputs[i], false);
		output_value->gen_outputs[i] = output_free(output_value->gen_outputs[i], false);
	}
	output_value->outputs = Free(output_value->outputs);
	output_value->gen_outputs = Free(output_value->gen_outputs);
	if (!keep_outer) {
		output_value = Free(output_value);
		return NULL;
	}
	output_value->num_of_outputs = 0;
	return NULL;
}

outcomes* outcomes_free(outcomes *outcomes, bool keep_outer) {
	if (outcomes == NULL) {
		return NULL;
	}
	outcomes->outcomes = Free(outcomes->outcomes);
	if (!keep_outer) {
		outcomes = Free(outcomes);
		return NULL;
	}
	outcomes->num_of_outcomes = 0;
	return NULL;
}

size_t gen_num_of_other_gate_permutations(size_t gate_count, size_t number_other_gates) {
	size_t result = gate_count;
	if (number_other_gates == 0) {
		return 0;
	}
	if (number_other_gates == 1) {
		return result;
	}
	for (size_t i = 0; i < number_other_gates - 1; i++) {
		result++;
		result *= gate_count;
	}
	return result;
}

void gen_other_gate_permutations(circuit *circuit) {
	size_t ONE = 1;
	size_t number_other_gates = circuit->num_of_other_gates;
	for (size_t i = 0; i < number_other_gates; i++) {
		circuit->gates[circuit->other_gate_indices[i]]->type = OR;
	}
	size_t gate_count = GATE_COUNT;
	size_t number_of_permutations = gen_num_of_other_gate_permutations(gate_count, number_other_gates);
	circuit->num_of_other_gate_permutations = number_of_permutations;
	circuit->other_gate_permutations = malloc(sizeof(size_t) * number_of_permutations);
	size_t curr_gate_index = number_other_gates - ONE;
	size_t last_index = 0;
	size_t j = 0;
	circuit->other_gate_permutations[0] = curr_gate_index;
	while (true) {
		j = last_index + ONE;
		for (size_t k = 0; k < gate_count - ONE; k++) {
			for (size_t i = 0; i <= last_index; i++) {
				circuit->other_gate_permutations[j] = circuit->other_gate_permutations[i];
				j++;
			}
		}
		if (curr_gate_index == 0) {
			break;
		}
		curr_gate_index--;
		circuit->other_gate_permutations[j] = curr_gate_index;
		last_index = j;
	}
}

size_t binary_to_gray(size_t binary, size_t num_of_bits) {
	size_t result = 0;
	size_t ONE = 1;
	size_t NUM_OF_BITS = num_of_bits;
	size_t MSB = binary >> (num_of_bits - ONE);
	if (MSB == ONE) {
		result = result | ONE;
	}
	size_t first = 0;
	size_t second = MSB;
	for (size_t i = 0; i < NUM_OF_BITS - ONE; i++) {
		first = second;
		num_of_bits--;
		binary = binary & ((ONE << num_of_bits) - ONE);
		second = binary >> (num_of_bits - ONE);
		result = result << ONE;
		result = result | (first ^ second);
	}
	return result;
}

outcomes* outcomes_builder(size_t num_of_bits) {
	outcomes *outcomes = outcomes_constructor();
	size_t ONE = 1;
	size_t num_of_outcomes = ONE << num_of_bits;
	outcomes->num_of_outcomes = num_of_outcomes;
	outcomes->outcomes = malloc(sizeof(size_t) * num_of_outcomes);
	for (size_t i = 0; i < num_of_outcomes; i++) {
		outcomes->outcomes[i] = i;
	}
	return outcomes;
}

input_value* input_value_builder(char *file) {
	input_value *input_value = input_value_constructor();
	FILE *fp = NULL;
	if ((fp = fopen(file, "r")) == NULL) {
		return NULL;
	}
	size_t num_of_input_vars = 0;
	size_t num_of_output_vars = 0;
	size_t line_counter = 0;
	readLine_char_firstD *line = NULL;
	while ((line = readLine(fp, line, 1, " ")) != NULL) {
		if ((*line).size == 0) {
			continue;
		}
		line_counter++;
		if (strcmp(line->i[0].j, "INPUTVAR") == 0) {
			sscanf(line->i[1].j, "%zu", &num_of_input_vars);
			continue;
		}
		if (strcmp(line->i[0].j, "OUTPUTVAR") == 0) {
			sscanf(line->i[1].j, "%zu", &num_of_output_vars);
			continue;
		}
		if (line_counter <= 3 + num_of_output_vars) {
			continue;
		}
		if (strcmp(line->i[0].j, "OR") == 0) {
			num_of_input_vars++;
			continue;
		}
		if (strcmp(line->i[0].j, "AND") == 0) {
			num_of_input_vars++;
			continue;
		}
		if (strcmp(line->i[0].j, "XOR") == 0) {
			num_of_input_vars++;
			continue;
		}
		if (strcmp(line->i[0].j, "NOT") == 0) {
			num_of_input_vars++;
			continue;
		}
		if (strcmp(line->i[0].j, "DECODER") == 0) {
			size_t ONE = 1;
			size_t num_of_decoder_inputs = 0;
			sscanf(line->i[1].j, "%zu", &num_of_decoder_inputs);
			num_of_input_vars += (ONE << num_of_decoder_inputs);
			continue;
		}
		if (strcmp(line->i[0].j, "MULTIPLEXER") == 0) {
			num_of_input_vars++;
			continue;
		}
		size_t ONE = 1;
		size_t num_of_other_vars = 0;
		sscanf(line->i[1].j, "%zu", &num_of_other_vars);
		num_of_input_vars += (num_of_other_vars - ONE); 
	}
	line = readLine_free(line);
	fclose(fp);
	fp = NULL;
	num_of_input_vars += 2;
	input_value->max_num_of_inputs = num_of_input_vars;
	input_value->inputs = malloc(sizeof(input*) * num_of_input_vars);
	for (size_t i = 0; i < num_of_input_vars; i++) {
		input_value->inputs[i] = input_constructor();
	}
	if ((fp = fopen(file, "r")) == NULL) {
		return NULL;
	}
	line = NULL;
	while ((line = readLine(fp, line, 1, " ")) != NULL) {
		if ((*line).size == 0) {
			continue;
		}
		if (strcmp(line->i[0].j, "INPUTVAR") == 0) {
			size_t number_input_vars = 0;
			sscanf(line->i[1].j, "%zu", &number_input_vars);
			input_value->original_num_of_inputs = number_input_vars;
			for (size_t i = 0; i < number_input_vars; i++) {
				input_value->current_num_of_inputs++;
				size_t string_length = strlen(line->i[2 + i].j) + 1;
				input_value->inputs[i]->input = malloc(sizeof(char) * string_length);
				strcpy(input_value->inputs[i]->input, line->i[2 + i].j);
			} 
			input_value->current_num_of_inputs += 2;
			size_t string_length = 2;
			input_value->inputs[number_input_vars]->input = malloc(sizeof(char) * string_length);
			strcpy(input_value->inputs[number_input_vars]->input, "0");
			input_value->inputs[number_input_vars]->value = 0;
			input_value->inputs[number_input_vars + 1]->input = malloc(sizeof(char) * string_length);
			strcpy(input_value->inputs[number_input_vars + 1]->input, "1");
			input_value->inputs[number_input_vars + 1]->value = 1;
			break;
		}
	}
	line = readLine_free(line);
	fclose(fp);
	fp = NULL;
	return input_value;
}

output_value* output_value_builder(char *file, input_value *input_value) {
	output_value *output_value = output_value_constructor();
	FILE *fp = NULL;
	if ((fp = fopen(file, "r")) == NULL) {
		return NULL;
	}
	size_t ONE = 1;
	size_t NUM_OF_VALUES = ONE << (input_value->original_num_of_inputs);
	size_t num_of_output_vars = 0;
	size_t line_counter = 0;
	readLine_char_firstD *line = NULL;
	while ((line = readLine(fp, line, 1, " ")) != NULL) {
		if ((*line).size == 0) {
			continue;
		}
		line_counter++;
		if (strcmp(line->i[0].j, "INPUTVAR") == 0) {
			continue;
		}
		if (strcmp(line->i[0].j, "OUTPUTVAR") == 0) {
			sscanf(line->i[1].j, "%zu", &num_of_output_vars);
			output_value->num_of_outputs = num_of_output_vars;
			output_value->outputs = malloc(sizeof(output*) * num_of_output_vars);
			output_value->gen_outputs = malloc(sizeof(output*) * num_of_output_vars);
			for (size_t i = 0; i < num_of_output_vars; i++) {
				output_value->outputs[i] = output_constructor();
				output_value->gen_outputs[i] = output_constructor();
				output_value->outputs[i]->num_of_values = NUM_OF_VALUES;
				output_value->gen_outputs[i]->num_of_values = NUM_OF_VALUES;
				output_value->outputs[i]->values = malloc(sizeof(size_t) * NUM_OF_VALUES);
				output_value->gen_outputs[i]->values = malloc(sizeof(size_t) * NUM_OF_VALUES);
				for (size_t j = 0; j < NUM_OF_VALUES; j++) {
					output_value->outputs[i]->values[j] = 0;
					output_value->gen_outputs[i]->values[j] = 0;
				}
			}
			for (size_t i = 0; i < num_of_output_vars; i++) {
				size_t string_length = strlen(line->i[2 + i].j) + 1;
				output_value->outputs[i]->output = malloc(sizeof(char) * string_length);
				output_value->gen_outputs[i]->output = malloc(sizeof(char) * string_length);
				strcpy(output_value->outputs[i]->output, line->i[2 + i].j);
				strcpy(output_value->gen_outputs[i]->output, line->i[2 + i].j);
			}
			continue;
		}
		if (line_counter == 3) {
			continue;
		}
		if (line_counter > 3 + num_of_output_vars) {
			break;
		}
		for (size_t i = 0; i < NUM_OF_VALUES; i++) {
			size_t val = 0;
			sscanf(line->i[1 + i].j, "%zu", &val);
			output_value->outputs[line_counter - 4]->values[i] = val;
		}
	}
	line = readLine_free(line);
	fclose(fp);
	fp = NULL;
	return output_value;
}

circuit* circuit_builder(char *file, input_value *input_value, output_value *output_value) {
	circuit *circuit = circuit_constructor();
	FILE *fp = NULL;
	if ((fp = fopen(file, "r")) == NULL) {
		return NULL;
	}
	size_t num_of_output_vars = output_value->num_of_outputs;
	size_t line_counter = 0;
	readLine_char_firstD *line = NULL;
	while ((line = readLine(fp, line, 1, " ")) != NULL) {
		if ((*line).size == 0) {
			continue;
		}
		line_counter++;
	}
	line = readLine_free(line);
	fclose(fp);
	fp = NULL;
	size_t num_of_gates = line_counter - 3 - num_of_output_vars;
	circuit->num_of_gates = num_of_gates;
	circuit->gates = malloc(sizeof(gate*) * num_of_gates);
	for (size_t i = 0; i < num_of_gates; i++) {
		circuit->gates[i] = gate_constructor();
	}
	if ((fp = fopen(file, "r")) == NULL) {
		return NULL;
	}
	char **tmp_input_vars = copy_array_of_strings(input_value->inputs, input_value->current_num_of_inputs, input_value->max_num_of_inputs);
	size_t tmp_current_num_of_input_vars = input_value->current_num_of_inputs;
	size_t num_of_other_gates = 0;
	line_counter = 0;
	line = NULL;
	while ((line = readLine(fp, line, 1, " ")) != NULL) {
		if ((*line).size == 0) {
			continue;
		}
		line_counter++;
		size_t gate_index = line_counter - 3 - num_of_output_vars - 1;
		if (line_counter <= 3 + num_of_output_vars) {
			continue;
		}
		if (strcmp(line->i[0].j, "OR") == 0) {
			circuit->gates[gate_index]->type = OR;
			circuit->gates[gate_index]->input_output->num_of_inputs = 2;
			circuit->gates[gate_index]->input_output->num_of_outputs = 1;
			circuit->gates[gate_index]->input_output->inputs = malloc(sizeof(char*) * 2);
			circuit->gates[gate_index]->input_output->outputs = malloc(sizeof(char*) * 1);
			size_t string_length = strlen(line->i[1].j) + 1;
			circuit->gates[gate_index]->input_output->inputs[0] = malloc(sizeof(char) * string_length);
			string_length = strlen(line->i[2].j) + 1;
			circuit->gates[gate_index]->input_output->inputs[1] = malloc(sizeof(char) * string_length);
			string_length = strlen(line->i[3].j) + 1;
			circuit->gates[gate_index]->input_output->outputs[0] = malloc(sizeof(char) * string_length);
			strcpy(circuit->gates[gate_index]->input_output->inputs[0], line->i[1].j);
			strcpy(circuit->gates[gate_index]->input_output->inputs[1], line->i[2].j);
			strcpy(circuit->gates[gate_index]->input_output->outputs[0], line->i[3].j);
			add_string_to_array(line->i[3].j, tmp_input_vars, tmp_current_num_of_input_vars);
			tmp_current_num_of_input_vars++;
			continue;
		}
		if (strcmp(line->i[0].j, "AND") == 0) {
			circuit->gates[gate_index]->type = AND;
			circuit->gates[gate_index]->input_output->num_of_inputs = 2;
			circuit->gates[gate_index]->input_output->num_of_outputs = 1;
			circuit->gates[gate_index]->input_output->inputs = malloc(sizeof(char*) * 2);
			circuit->gates[gate_index]->input_output->outputs = malloc(sizeof(char*) * 1);
			size_t string_length = strlen(line->i[1].j) + 1;
			circuit->gates[gate_index]->input_output->inputs[0] = malloc(sizeof(char) * string_length);
			string_length = strlen(line->i[2].j) + 1;
			circuit->gates[gate_index]->input_output->inputs[1] = malloc(sizeof(char) * string_length);
			string_length = strlen(line->i[3].j) + 1;
			circuit->gates[gate_index]->input_output->outputs[0] = malloc(sizeof(char) * string_length);
			strcpy(circuit->gates[gate_index]->input_output->inputs[0], line->i[1].j);
			strcpy(circuit->gates[gate_index]->input_output->inputs[1], line->i[2].j);
			strcpy(circuit->gates[gate_index]->input_output->outputs[0], line->i[3].j);
			add_string_to_array(line->i[3].j, tmp_input_vars, tmp_current_num_of_input_vars);
			tmp_current_num_of_input_vars++;
			continue;
		}
		if (strcmp(line->i[0].j, "XOR") == 0) {
			circuit->gates[gate_index]->type = XOR;
			circuit->gates[gate_index]->input_output->num_of_inputs = 2;
			circuit->gates[gate_index]->input_output->num_of_outputs = 1;
			circuit->gates[gate_index]->input_output->inputs = malloc(sizeof(char*) * 2);
			circuit->gates[gate_index]->input_output->outputs = malloc(sizeof(char*) * 1);
			size_t string_length = strlen(line->i[1].j) + 1;
			circuit->gates[gate_index]->input_output->inputs[0] = malloc(sizeof(char) * string_length);
			string_length = strlen(line->i[2].j) + 1;
			circuit->gates[gate_index]->input_output->inputs[1] = malloc(sizeof(char) * string_length);
			string_length = strlen(line->i[3].j) + 1;
			circuit->gates[gate_index]->input_output->outputs[0] = malloc(sizeof(char) * string_length);
			strcpy(circuit->gates[gate_index]->input_output->inputs[0], line->i[1].j);
			strcpy(circuit->gates[gate_index]->input_output->inputs[1], line->i[2].j);
			strcpy(circuit->gates[gate_index]->input_output->outputs[0], line->i[3].j);
			add_string_to_array(line->i[3].j, tmp_input_vars, tmp_current_num_of_input_vars);
			tmp_current_num_of_input_vars++;
			continue;
		}
		if (strcmp(line->i[0].j, "NOT") == 0) {
			circuit->gates[gate_index]->type = NOT;
			circuit->gates[gate_index]->input_output->num_of_inputs = 1;
			circuit->gates[gate_index]->input_output->num_of_outputs = 1;
			circuit->gates[gate_index]->input_output->inputs = malloc(sizeof(char*) * 1);
			circuit->gates[gate_index]->input_output->outputs = malloc(sizeof(char*) * 1);
			size_t string_length = strlen(line->i[1].j) + 1;
			circuit->gates[gate_index]->input_output->inputs[0] = malloc(sizeof(char) * string_length);
			string_length = strlen(line->i[2].j) + 1;
			circuit->gates[gate_index]->input_output->outputs[0] = malloc(sizeof(char) * string_length);
			strcpy(circuit->gates[gate_index]->input_output->inputs[0], line->i[1].j);
			strcpy(circuit->gates[gate_index]->input_output->outputs[0], line->i[2].j);
			add_string_to_array(line->i[2].j, tmp_input_vars, tmp_current_num_of_input_vars);
			tmp_current_num_of_input_vars++;
			continue;
		}
		if (strcmp(line->i[0].j, "DECODER") == 0) {
			circuit->gates[gate_index]->type = DECODER;
			size_t ONE = 1;
			size_t num_of_decoder_inputs = 0;
			sscanf(line->i[1].j, "%zu", &num_of_decoder_inputs);
			size_t num_of_decoder_outputs = ONE << num_of_decoder_inputs;
			circuit->gates[gate_index]->input_output->num_of_inputs = num_of_decoder_inputs;
			circuit->gates[gate_index]->input_output->num_of_outputs = num_of_decoder_outputs;
			circuit->gates[gate_index]->input_output->inputs = malloc(sizeof(char*) * num_of_decoder_inputs);
			circuit->gates[gate_index]->input_output->outputs = malloc(sizeof(char*) * num_of_decoder_outputs);
			size_t string_length = 0;
			for (size_t i = 0; i < num_of_decoder_inputs; i++) {
				string_length = strlen(line->i[2 + i].j) + 1;
				circuit->gates[gate_index]->input_output->inputs[i] = malloc(sizeof(char) * string_length);
				strcpy(circuit->gates[gate_index]->input_output->inputs[i], line->i[2 + i].j);
			}
			for (size_t i = 0; i < num_of_decoder_outputs; i++) {
				string_length = strlen(line->i[2 + num_of_decoder_inputs + i].j) + 1;
				circuit->gates[gate_index]->input_output->outputs[i] = malloc(sizeof(char) * string_length);
				strcpy(circuit->gates[gate_index]->input_output->outputs[i], line->i[2 + num_of_decoder_inputs + i].j);
				add_string_to_array(line->i[2 + num_of_decoder_inputs + i].j, tmp_input_vars, tmp_current_num_of_input_vars);
				tmp_current_num_of_input_vars++;
			}
			continue;
		}
		if (strcmp(line->i[0].j, "MULTIPLEXER") == 0) {
			circuit->gates[gate_index]->type = MULTIPLEXER;
			size_t num_of_multi_inputs = 0;
			sscanf(line->i[1].j, "%zu", &num_of_multi_inputs);
			size_t num_of_multi_selectors = logarithm_base_2_floored(num_of_multi_inputs);
			circuit->gates[gate_index]->input_output->num_of_inputs = num_of_multi_inputs;
			circuit->gates[gate_index]->input_output->num_of_outputs = 1;
			circuit->gates[gate_index]->input_output->num_of_selectors = num_of_multi_selectors;
			circuit->gates[gate_index]->input_output->inputs = malloc(sizeof(char*) * num_of_multi_inputs);
			circuit->gates[gate_index]->input_output->outputs = malloc(sizeof(char*) * 1);
			circuit->gates[gate_index]->input_output->selectors = malloc(sizeof(char*) * num_of_multi_selectors);
			size_t string_length = 0;
			for (size_t i = 0; i < num_of_multi_inputs; i++) {
				string_length = strlen(line->i[2 + i].j) + 1;
				circuit->gates[gate_index]->input_output->inputs[i] = malloc(sizeof(char) * string_length);
				strcpy(circuit->gates[gate_index]->input_output->inputs[i], line->i[2 + i].j);
			}
			for (size_t i = 0; i < num_of_multi_selectors; i++) {
				string_length = strlen(line->i[2 + num_of_multi_inputs + i].j) + 1;
				circuit->gates[gate_index]->input_output->selectors[i] = malloc(sizeof(char) * string_length);
				strcpy(circuit->gates[gate_index]->input_output->selectors[i], line->i[2 + num_of_multi_inputs + i].j);
			}
			string_length = strlen(line->i[2 + num_of_multi_inputs + num_of_multi_selectors].j) + 1;
			circuit->gates[gate_index]->input_output->outputs[0] = malloc(sizeof(char) * string_length);
			strcpy(circuit->gates[gate_index]->input_output->outputs[0], line->i[2 + num_of_multi_inputs + num_of_multi_selectors].j);
			add_string_to_array(line->i[2 + num_of_multi_inputs + num_of_multi_selectors].j, tmp_input_vars, tmp_current_num_of_input_vars);
			tmp_current_num_of_input_vars++;
			continue;
		}
		num_of_other_gates++;
		circuit->gates[gate_index]->type = OTHER;
		size_t string_length = 0;
		size_t num_of_total_vars = 0;
		sscanf(line->i[1].j, "%zu", &num_of_total_vars);
		size_t num_of_other_inputs = 0;
		size_t num_of_other_outputs = 0;
		for (size_t i = 0; i < num_of_total_vars; i++) {
			if (array_contains_string(line->i[2 + i].j, tmp_input_vars, tmp_current_num_of_input_vars)) {
				num_of_other_inputs++;
			} else {
				num_of_other_outputs++;
			}
		}
		circuit->gates[gate_index]->input_output->num_of_inputs = num_of_other_inputs;
		circuit->gates[gate_index]->input_output->num_of_outputs = num_of_other_outputs;
		circuit->gates[gate_index]->input_output->inputs = malloc(sizeof(char*) * num_of_other_inputs);
		circuit->gates[gate_index]->input_output->outputs = malloc(sizeof(char*) * num_of_other_outputs);
		size_t input_counter = 0;
		size_t output_counter = 0;
		for (size_t i = 0; i < num_of_total_vars; i++) {
			string_length = strlen(line->i[2 + i].j) + 1;
			if (array_contains_string(line->i[2 + i].j, tmp_input_vars, tmp_current_num_of_input_vars)) {
				circuit->gates[gate_index]->input_output->inputs[input_counter] = malloc(sizeof(char) * string_length);
				strcpy(circuit->gates[gate_index]->input_output->inputs[input_counter], line->i[2 + i].j);
				input_counter++;
			} else {
				circuit->gates[gate_index]->input_output->outputs[output_counter] = malloc(sizeof(char) * string_length);
				strcpy(circuit->gates[gate_index]->input_output->outputs[output_counter], line->i[2 + i].j);
				output_counter++;
				add_string_to_array(line->i[2 + i].j, tmp_input_vars, tmp_current_num_of_input_vars);
				tmp_current_num_of_input_vars++;
			}
		}
	}
	line = readLine_free(line);
	fclose(fp);
	fp = NULL;
	tmp_input_vars = free_array_of_strings(tmp_input_vars, tmp_current_num_of_input_vars);
	circuit->num_of_other_gates = num_of_other_gates;
	circuit->other_gate_indices = malloc(sizeof(size_t) * num_of_other_gates);
	size_t curr_index = 0;
	for (size_t i = 0; i < num_of_gates; i++) {
		if (circuit->gates[i]->type == OTHER) {
			circuit->other_gate_indices[curr_index] = i;
			curr_index++;
		}
	}
	gen_other_gate_permutations(circuit);
	return circuit;
}

void reset_input_value(input_value *input_value, size_t input_vars_permutation) {
	size_t ONE = 1;
	size_t num_of_bits = input_value->original_num_of_inputs;
	size_t curr_num_of_inputs = input_value->current_num_of_inputs;
	for (size_t i = num_of_bits + 2; i < curr_num_of_inputs; i++) {
		input_free(input_value->inputs[i], true);
	}
	size_t bit_counter = num_of_bits - ONE;
	input_value->current_num_of_inputs = num_of_bits + 2;
	for (size_t i = 0; i < num_of_bits; i++) {
		input_value->inputs[i]->value = (input_vars_permutation & (ONE << bit_counter)) >> bit_counter;
		bit_counter--;
	}
}

void reset_output_value(output_value *output_value) {
	size_t num_of_outputs = output_value->num_of_outputs;
	for (size_t i = 0; i < num_of_outputs; i++) {
		size_t num_of_values = output_value->gen_outputs[i]->num_of_values;
		for (size_t j = 0; j < num_of_values; j++) {
			output_value->gen_outputs[i]->values[j] = 0;
		}
	}
}

bool gen_outputs_correct(output_value *output_value) {
	size_t num_of_outputs = output_value->num_of_outputs;
	for (size_t i = 0; i < num_of_outputs; i++) {
		size_t num_of_values = output_value->gen_outputs[i]->num_of_values;
		for (size_t j = 0; j < num_of_values; j++) {
			if (output_value->gen_outputs[i]->values[j] != output_value->outputs[i]->values[j]) {
				return false;
			}
		}
	}
	return true;
}

size_t get_value_of_input_var(char *input_var, input_value *input_value) {
	size_t num_of_inputs = input_value->current_num_of_inputs;
	for (size_t i = 0; i < num_of_inputs; i++) {
		if (strcmp(input_value->inputs[i]->input, input_var) == 0) {
			return input_value->inputs[i]->value;
		}
	}
	return 0;
}

bool output_var_is_final(char *output_var, output_value *output_value) {
	size_t num_of_outputs = output_value->num_of_outputs;
	for (size_t i = 0; i < num_of_outputs; i++) {
		if (strcmp(output_value->outputs[i]->output, output_var) == 0) {
			return true;
		}
	}
	return false;
}

void update_final_output_var(char *final_output_var, size_t value, size_t index_of_current_input_permutation, output_value *output_value) {
	size_t num_of_outputs = output_value->num_of_outputs;
	for (size_t i = 0; i < num_of_outputs; i++) {
		if (strcmp(output_value->gen_outputs[i]->output, final_output_var) == 0) {
			output_value->gen_outputs[i]->values[index_of_current_input_permutation] = value;
			return;
		}
	}
	return;
}

void add_output_to_inputs(char *output_var, size_t value, input_value *input_value) {
	size_t curr_num_of_inputs = input_value->current_num_of_inputs;
	input_value->current_num_of_inputs++;
	input_value->inputs[curr_num_of_inputs]->input = malloc(sizeof(char) * (strlen(output_var) + 1));
	strcpy(input_value->inputs[curr_num_of_inputs]->input, output_var);
	input_value->inputs[curr_num_of_inputs]->value = value;
}

bool increment_other_gate_return_skip(circuit *circuit, size_t index_of_other_gate_permutation) {
	gate *gate = circuit->gates[circuit->other_gate_indices[circuit->other_gate_permutations[index_of_other_gate_permutation]]];
	if (gate->type == MULTIPLEXER) {
		gate->type = OR;
		return true;
	}
	gate->type = gate->type + 1;
	return false;
}

size_t get_number_of_selectors(size_t num_of_inputs) {
	size_t ONE = 1;
	size_t result = ONE;
	if (num_of_inputs < 3) {
		return 0;
	}
	while (((ONE << result) + result) < num_of_inputs) {
		result++;
	}
	if (((ONE << result) + result) == num_of_inputs) {
		return result;
	}
	return 0;
}

bool OR_gate(input_output *IO, input_value *input_value, output_value *output_value, size_t index_of_input_permu) {
	if (IO->num_of_inputs != 2 || IO->num_of_outputs != 1 || IO->num_of_selectors != 0) {
		return false;
	}
	size_t input_1 = get_value_of_input_var(IO->inputs[0], input_value);
	size_t input_2 = get_value_of_input_var(IO->inputs[1], input_value);
	size_t result = input_1 | input_2;
	if (output_var_is_final(IO->outputs[0], output_value)) {
		update_final_output_var(IO->outputs[0], result, index_of_input_permu, output_value);
	} else {
		add_output_to_inputs(IO->outputs[0], result, input_value);
	}
	return true;
}

bool AND_gate(input_output *IO, input_value *input_value, output_value *output_value, size_t index_of_input_permu) {
	if (IO->num_of_inputs != 2 || IO->num_of_outputs != 1 || IO->num_of_selectors != 0) {
		return false;
	}
	size_t input_1 = get_value_of_input_var(IO->inputs[0], input_value);
	size_t input_2 = get_value_of_input_var(IO->inputs[1], input_value);
	size_t result = input_1 & input_2;
	if (output_var_is_final(IO->outputs[0], output_value)) {
		update_final_output_var(IO->outputs[0], result, index_of_input_permu, output_value);
	} else {
		add_output_to_inputs(IO->outputs[0], result, input_value);
	}
	return true;
}

bool XOR_gate(input_output *IO, input_value *input_value, output_value *output_value, size_t index_of_input_permu) {
	if (IO->num_of_inputs != 2 || IO->num_of_outputs != 1 || IO->num_of_selectors != 0) {
		return false;
	}
	size_t input_1 = get_value_of_input_var(IO->inputs[0], input_value);
	size_t input_2 = get_value_of_input_var(IO->inputs[1], input_value);
	size_t result = input_1 ^ input_2;
	if (output_var_is_final(IO->outputs[0], output_value)) {
		update_final_output_var(IO->outputs[0], result, index_of_input_permu, output_value);
	} else {
		add_output_to_inputs(IO->outputs[0], result, input_value);
	}
	return true;
}

bool NOT_gate(input_output *IO, input_value *input_value, output_value *output_value, size_t index_of_input_permu) {
	if (IO->num_of_inputs != 1 || IO->num_of_outputs != 1 || IO->num_of_selectors != 0) {
		return false;
	}
	size_t input_1 = get_value_of_input_var(IO->inputs[0], input_value);
	size_t result = !input_1;
	if (output_var_is_final(IO->outputs[0], output_value)) {
		update_final_output_var(IO->outputs[0], result, index_of_input_permu, output_value);
	} else {
		add_output_to_inputs(IO->outputs[0], result, input_value);
	}
	return true;
}

bool DECODER_gate(input_output *IO, input_value *input_value, output_value *output_value, size_t index_of_input_permu) {
	size_t ONE = 1;
	if (IO->num_of_inputs < 1 || IO->num_of_outputs != (ONE << IO->num_of_inputs) || IO->num_of_selectors != 0) {
		return false;
	}
	size_t num_of_bits = IO->num_of_inputs;
	outcomes *outcomes = outcomes_builder(num_of_bits);
	size_t num_of_outcomes = outcomes->num_of_outcomes;
	for (size_t i = 0; i < num_of_outcomes; i++) {
		size_t correct = 1;
		size_t bit_counter = num_of_bits - ONE;
		for (size_t j = 0; j < num_of_bits; j++) {
			if (get_value_of_input_var(IO->inputs[j], input_value) != (outcomes->outcomes[i] & (ONE << bit_counter)) >> bit_counter) {
				correct = 0;
				break;
			}
			bit_counter--;
		}
		if (correct == 0) {
			if (output_var_is_final(IO->outputs[i], output_value)) {
				update_final_output_var(IO->outputs[i], 0, index_of_input_permu, output_value);
			} else {
				add_output_to_inputs(IO->outputs[i], 0, input_value);
			}
			continue;
		}
		if (output_var_is_final(IO->outputs[i], output_value)) {
			update_final_output_var(IO->outputs[i], 1, index_of_input_permu, output_value);
		} else {
			add_output_to_inputs(IO->outputs[i], 1, input_value);
		}
	}
	outcomes = outcomes_free(outcomes, false);
	return true;
}

bool MULTIPLEXER_gate(input_output *IO, input_value *input_value, output_value *output_value, size_t index_of_input_permu) {
	size_t ONE = 1;
	if (IO->num_of_inputs < 2 || IO->num_of_outputs != ONE) {
		return false;
	}
	size_t num_of_inputs = IO->num_of_inputs;
	size_t num_of_selectors = IO->num_of_selectors;
	size_t test_num_of_selectors = get_number_of_selectors(IO->num_of_inputs);
	size_t access_back = 0;
	if (num_of_selectors == 0) {
		if (test_num_of_selectors == 0) {
			return false;
		} else {
			num_of_selectors = test_num_of_selectors;
			access_back = 1;
		}
	}
	size_t num_of_bits = num_of_selectors;
	outcomes *outcomes = outcomes_builder(num_of_bits);
	size_t num_of_outcomes = outcomes->num_of_outcomes;
	for (size_t i = 0; i < num_of_outcomes; i++) {
		size_t correct = 1;
		size_t bit_counter = num_of_bits - ONE;
		for (size_t j = 0, k = num_of_inputs - num_of_bits; j < num_of_bits; j++, k++) {
			char *selector = NULL;
			if (access_back == 1) {
				selector = IO->inputs[k];
			} else {
				selector = IO->selectors[j];
			}
			if (get_value_of_input_var(selector, input_value) != (outcomes->outcomes[i] & (ONE << bit_counter)) >> bit_counter) {
				correct = 0;
				break;
			}
			bit_counter--;
		}
		if (correct == 0) {
			continue;
		}
		if (output_var_is_final(IO->outputs[0], output_value)) {
			update_final_output_var(IO->outputs[0], get_value_of_input_var(IO->inputs[i], input_value), index_of_input_permu, output_value);
		} else {
			add_output_to_inputs(IO->outputs[0], get_value_of_input_var(IO->inputs[i], input_value), input_value);
		}
		break;
	}
	outcomes = outcomes_free(outcomes, false);
	return true;
}


