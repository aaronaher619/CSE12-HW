/**************************************************************************** 
 
                                                        Aaron Hernandez 
                                                        CSE 12, Spring 2017 
                                                        April 19, 2017 
                                                        cs12xcb 
                                Assignment Three 
 
File Name:      stack.c 
Description:    This program reads input from the user one char at a time.
                From that input you can create a stack that stores a x amount
                of longs. You can also pop, top, push, delete stack, empty
                stack, print stack, ask if full, ask if empty, ask how many
                number of longs the stack has and print debug info. 
 
****************************************************************************/  

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of next available space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "get_occupancy check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                        "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                        "Attempt to write to a non-existent stack!!!\n";

/* Debug messages */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) {
        debug = FALSE;
}

void debug_on (void) {
        debug = TRUE;
}

/* start of true stack code */

/*-------------------------------------------------------------------------- 
Function Name:          delete_Stack 
Purpose:                Delete the current stack 
Description:            This function checks to see if the pointer is null
                        or if the pointer's pointer is null. If it fails
			then it prints error message and breaks from the main
			loop.If it passes then it frees the memory associated
			with the stack and resets the pointer to point to
			null. 
Input:                  None. 
Result:                 The stack is deallocated 
--------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {

  /*Checks if the pointer is null.
    If so, return an error*/
  if(*spp == NULL){
    fprintf(stderr, DELETE_NONEXIST);
    return;
  }

  /*Checks  if the pointer's pointer is null.
    If so, return an error*/
  if(spp == NULL){
    fprintf(stderr, DELETE_NONEXIST);
    return;
  }

  /*Frees the memory of stack and sets the pointer
    to null*/
  free(*spp - STACK_OFFSET);
  *spp = NULL;
  
  /*The debug check*/
  if(debug){
    fprintf(stderr, DEALLOCATE, (long) stack_counter);
  }

  stack_counter = 0;
}

/*--------------------------------------------------------------------------
Function Name:          empty_Stack
Purpose:                Emptys the current stack's numbers
Description:            This function checks to see if the stack is null.
                        If it fails then it prints error message and breaks
			from main loop. If it passes then it resets the
			STACK_POINTER_INDEX to the beinning of the stack.
			
Input:                  Pointer to the stack
Result:                 The stack's longs are emptyed
--------------------------------------------------------------------------*/
void empty_Stack (Stack * this_Stack) {
  /*Checks if the stack is null. If so, return error*/
  if(this_Stack == NULL){
    fprintf(stderr, EMPTY_NONEXIST);
    return;
  }

  /*sets STACK_POINTER_INDEX back to the bottom of the stack*/
  else{
    this_Stack[STACK_POINTER_INDEX] = 0;
  }
}
        
/*--------------------------------------------------------------------------
Function Name:          isempty_Stack
Purpose:                Checks if the stack is empty of longs
Description:            This function checks to see if the stack is mull.
                        If it fails then it prints error message and breaks
			from the main loop. If it passes then it returns true if the
			stack is empty of longs and false otherwise. 
Input:                  Pointer to the stack
Result:                 Answer to wherther or not the stack is empty.
--------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_Stack) {

  /*Checks if the stack is null and prints error if so.*/
  if(this_Stack == NULL){
    fprintf(stderr, ISEMPTY_NONEXIST);
    return 0;
  }
    
  /*If the STACK_POINTER_INDEX is zero then the stack being empty is true*/
  if (this_Stack[STACK_POINTER_INDEX] == 0){
    return 1;
  }

  /*If stack not empty then return false*/
  else{
    return 0;
  }
}

/*--------------------------------------------------------------------------
Function Name:          isfull_Stack
Purpose:                Checks if the stack is full
Description:            This function checks to see if the stack is null.
                        If it fails then it prints error message and breaks
			from main loop. If it passes then it returns true if
			the stack is full and false otherwise.
Input:                  Pointer to the stack
Result:                 Answer to wherther or not the stack is full.
--------------------------------------------------------------------------*/
long isfull_Stack (Stack * this_Stack) {

  /*Checks if the stack is null and prints error if so.*/
  if(this_Stack == NULL){
    fprintf(stderr, ISFULL_NONEXIST);
    return 0;
  }

  /*If the stack pointer index is an index greater than what the stack can
    hold then return true that the stack is full*/
  if ((this_Stack[STACK_POINTER_INDEX] + 1) >  this_Stack[STACK_SIZE_INDEX]){
    return 1;
  }

  /*If stack not empty then return false*/
  else{
    return 0;
  }
}

/*--------------------------------------------------------------------------
Function Name:          new_Stack
Purpose:                Creates a new stack
Description:            It creates mempory (according the number of
                        stacksize)and gives it to stack. Once done it sets
			STACK_POINTER_INDEX + STACK_OFFSET to 0,
			STACK_SIZE_INDEX + STACK_OFFSET to stacksize,
			STACK_COUNT_INDEX + STACK_OFFSET to stack_couner.
Input:                  long of the stacksize
Result:                 A pointer to the memory given to stack
--------------------------------------------------------------------------*/
Stack * new_Stack (unsigned long stacksize) {

  /*Gives the stack memory from malloc and accounts for the offset*/
  Stack* stack = (Stack*) malloc(((stacksize + STACK_OFFSET) * sizeof(long)));

  /*Debug message*/
  if(debug && stack_counter){
    fprintf(stderr, DEALLOCATE, (long)stack_counter);
    stack_counter = 0;
  }

  /*Adds one to stack counter, sets the pointer index to the beginning
    of the stack, the size index to the stack size and count index to stack
    counter. All accounting for the offset.*/
  ++stack_counter;
  stack[STACK_POINTER_INDEX + STACK_OFFSET] = 0;
  stack[STACK_SIZE_INDEX + STACK_OFFSET] = stacksize;
  stack[STACK_COUNT_INDEX + STACK_OFFSET] = stack_counter;
  
  /*Debug message*/
  if(debug){
    fprintf(stderr, ALLOCATED, (long)stack_counter);
  }

  /*return the pointer to the stack's memory plus the offset*/
  return stack + STACK_OFFSET;
}

/*--------------------------------------------------------------------------
Function Name:          get_occupancy
Purpose:                Get the number of longs stored in the stack
Description:            This function checks to see if the stack is null. If
                        it fails then it prints error message and breaks
			from the main loop. If it passes then it returns the
			number of longs stored in the current stack,
Input:                  Pointer to the stack
Result:                 Number of longs in stack
--------------------------------------------------------------------------*/
long get_occupancy (Stack * this_Stack) {

  /*Checks if the stack is null and prints error message if so*/
  if(this_Stack == NULL){
    fprintf(stderr, NUM_NONEXIST);
    return 0;
  }

  /*Returns the pointer index which is also the number of longs stack has*/
  return this_Stack[STACK_POINTER_INDEX];
}

/*--------------------------------------------------------------------------
Function Name:          pop
Purpose:                To remove a long to the stack
Description:            This function checks to see if the stack is null or
                        if item is null. If it fails then it prints error
			message and breaks from the main loop.
			If it passes then it checks if the stack is empty.
			If it fails it prints an error message and if it
			passes it removes one from the STACK_POINTER_INDEX
			and sets item to the long that was removed.
Input:                  Pointer to the stack and pointer to the long
Result:                 Removes a long to the stack
--------------------------------------------------------------------------*/
long pop (Stack * this_Stack, long * item) {

  /*Checks if stack is null and prints error if so*/
  if(this_Stack == NULL){
    fprintf(stderr, POP_NONEXIST);
    return 0;
  }

  /*Checks if item is null and prints error if so*/
  if(item == NULL){
    fprintf(stderr, INCOMING_NONEXIST);
    return 0;
  }

  /*Checks if stack is empty and prints error if so*/
  if(isempty_Stack(this_Stack)){
    fprintf(stderr, POP_EMPTY);
    return 0;
  }

  /*Sets item to the topmost long of the stack and removes one from the 
    pointer index*/
  *item = this_Stack[this_Stack[STACK_POINTER_INDEX] - 1]; 
  this_Stack[STACK_POINTER_INDEX] = this_Stack[STACK_POINTER_INDEX] - 1;  

  /*Debug Message*/
  if(debug){
    fprintf(stderr, POP, this_Stack[STACK_COUNT_INDEX], *item);
  }

  /*Returns true*/
  return 1;
}

/*--------------------------------------------------------------------------
Function Name:          puch
Purpose:                To add a long to the stack
Description:            This function checks to see if the stack is null or.
                        If it fails then it prints error message and breaks
			from the main loop. If it passes then it checks if
			the stack is full. If it fails it prints an error
			message and if it passes it adds one to the stack
			and adds one to the STACK_POINTER_INDEX.
Input:                  Pointer to the stack and a long
Result:                 Adds a long to the stack
--------------------------------------------------------------------------*/
long push (Stack * this_Stack, long item) {

  /*Checks if stack is null and prints error if so.*/
  if(this_Stack == NULL){
    fprintf(stderr, PUSH_NONEXIST);
    return 0;
  }

  /*Checks if  item is null and prints error if so.*/
  if(isfull_Stack(this_Stack)){
    fprintf(stderr, PUSH_FULL);
    return 0;
  }
  
  /*Adds the item which is a long to the stack and adds one to the 
    pointer index*/
  this_Stack[this_Stack[STACK_POINTER_INDEX]] = item;
  this_Stack[STACK_POINTER_INDEX] = this_Stack[STACK_POINTER_INDEX] + 1;
  
  /*Debug message*/
  if(debug){
    fprintf(stderr, PUSH, this_Stack[STACK_COUNT_INDEX], item);
  }    

  /*Returns true*/
  return 1;
}

/*--------------------------------------------------------------------------
Function Name:          top
Purpose:                Get the long at the top of the stack
Description:            This function checks to see if the stack is null or
                        if item is null. If it fails then it prints error
                        message and breaks from the main loop. If it passes
			then it checks if the stack is empty. If it fails it
			prints an error message and if it passes it sets
			item to the long at the top of the stack.
Input:                  Pointer to the stack and pointer to the long
Result:                 Eits a the item parameter to the topmost long of 
                        the stack.
--------------------------------------------------------------------------*/
long top (Stack * this_Stack, long * item) {

  /*Checks if the stack is null and printss error if so*/
  if(this_Stack == NULL){
    fprintf(stderr, TOP_NONEXIST);
    return 0;
  }

  /*Checks if item is null anf prints error if so*/
  if(item == NULL){
    fprintf(stderr, INCOMING_NONEXIST);
    return 0;
  }

  /*Checks if stack is empty and prints error if so*/
  if(isempty_Stack(this_Stack)){
    fprintf(stderr, TOP_EMPTY);
    return 0;
  }

  /*Sets item to the long at the top of the stack*/
  *item = this_Stack[this_Stack[STACK_POINTER_INDEX] - 1];
  
  /*Debug message*/
  if(debug){
    fprintf(stderr, TOP, this_Stack[STACK_COUNT_INDEX], *item);
  }

  /*Returns true*/
  return 1;
}

FILE * write_Stack (Stack * this_Stack, FILE * stream) {

        long index = 0;         /* index into the stack */

        if (this_Stack == NULL) {
                fprintf (stderr, WRITE_NONEXIST_STACK);
                return stream;
        }

        if (stream == NULL) {
                fprintf (stderr, WRITE_NONEXIST_FILE);
                return stream;
        }
                
        if (stream == stderr)
                fprintf (stream, "Stack has %ld items in it.\n",
                        get_occupancy (this_Stack));

        for (index = STACK_COUNT_INDEX + STACK_OFFSET;
                index < get_occupancy (this_Stack); index++) {

                if (stream == stderr)
                        fprintf (stream, "Value on stack is |0x%lx|\n",
                                this_Stack[index]);
                else {
                        if (this_Stack[index] < 0)
                                fprintf (stream, "%c ",
                                        (char) this_Stack[index]);
                        else
                                fprintf (stream, "%ld ", this_Stack[index]);
                }
        }

        return stream;
}
