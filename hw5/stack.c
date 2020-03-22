#include <stdio.h>
#include "list.h"
#include "stack.h"

/*--------------------------------------------------------------------------                                                                                 
Function Name:          delete_Stack                                                                                                                         
Purpose:                Delete the current stack                                                                                                             
Description:            Calls delete_List from list.c                                                                                                
Input:                  Address of the HEAP memory to delete                                                                                        
Result:                 The stack is deallocated                                                                                                             
--------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {
        delete_List (spp);
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          isempty_Stack                                                                                                                        
Purpose:                Checks if the stack is empty of longs                                                                                                
Description:            Calls isempty_List from list.c                                                                                         
Input:                  Pointer to the stack                                                                                                                 
Result:                 returns 1 id success otherwise 0                                                                                        
--------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_stack) {
        return isempty_List (this_stack);
}

/*--------------------------------------------------------------------------                                                                                
Function Name:          new_Stack                                                                                                                            
Purpose:                Creates a new list                                                                                                                  
Description:            Calls new_List from list.c                                                                                    
Input:                  function pointers to copy_func, delete_func, 
                        write_func and is_greater_than_func                                                                                                  Result:                 A pointer to the memory given to stack                                                                                               
--------------------------------------------------------------------------*/
Stack * new_Stack (void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) {

        return new_List (copy_func, delete_func, 
		is_greater_than_func, write_func);
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          pop                                                                                                                                  
Purpose:                To remove a long to the stack                                                                                                        
Description:            Calls remove_List from list.c                                                                                          
Input:                  Pointer to the stack                                                                                    
Result:                 returns a pointer to the element that was removed                                                                        
--------------------------------------------------------------------------*/
void * pop (Stack * this_stack) {
        return remove_List (this_stack, END);
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          push                                                                                                                                 
Purpose:                To add a long to the stack                                                                                                           
Description:            Calls insert from list.c.                                                                                             
Input:                  Pointer to the stack and a pointer to the element                                                                       
Result:                 returns the element that was pushed                                                                        
--------------------------------------------------------------------------*/
long push (Stack * this_stack, void * element) {
        return insert (this_stack, element, END);
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          top                                                                                                                                  
Purpose:                Get the long at the top of the stack                                                                                                 
Description:            Calls view from the list.c                                                                                 
Input:                  Pointer to the stack                                                                                         
Result:                 return pointer from calling view                                                                  
---------------------------------------------------------------------------*/
void * top (Stack * this_stack) {
        return view (this_stack, END);
}

/*-------------------------------------------------------------------------
Function Name:          write_Stack
Purpose:                Outputs the nodes in a list list
Description:            Calls write_List
Input:                  Pointer to stack and pointer to stream
result:                 return the stream pointer
---------------------------------------------------------------------------*/
FILE * write_Stack (Stack * this_stack, FILE * stream) {
        return write_List (this_stack, stream);
}
