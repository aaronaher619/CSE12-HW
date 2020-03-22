/****************************************************************************                                                                                
                                                                                                                                                             
                                                        Aaron Hernandez                                                                                      
                                                        CSE 12, Spring 2017                                                                                  
                                                        May 2 , 2017                                                                                       
                                                        cs12xcb                                                                                              
                                Assignment Five                                                                                                             
                                                                                                                                                             
File Name:      list.c                                                                                                                                      
Description:    This program creates a list of longs and then you can insert
                longs in this list. You can then look up the start or end of 
                the list and also remove the start or end of the list. You 
                can also check if the list is empty and write out the longs
                in correct order or in reverse order. On exit it deletes the 
                list and the nodes holding the longs.
****************************************************************************/
#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "list.h"

typedef struct Node {
        struct Node * pre;      /* how to access the prior Node */
        struct Node * next;     /* how to access the next Node */
        void * data;            /* the data to store */
} Node;

typedef struct List {
        Node * front;             /* the front of the list */
        long list_count;        /* which list is it */
        long occupancy;
        void *(*copy_func) (void *);
        void (*delete_func) (void *);
        long (*is_greater_than_func) (void *, void *);
        FILE *(*write_func) (void *, FILE *);
} List;

/* private Node function declarations */
static void delete_Node (Node **, void (*delete_func) (void *));
static Node * insert_Node (Node *, void *, void *(*copy_func) (void *)); 
static Node * locate (List * this_list, void * element, long * num);
static Node * new_Node (void *, void *(*copy_func) (void *));
static void * remove_Node (Node *);
static void * view_Node (Node *); 
static FILE * write_Node (Node *, FILE *, FILE *(*write_func) (void *, FILE *));

/* catastrophic error messages */
static const char ADNEXT_NONEXIST[] = 
                "Advance next from non-existent list!!!\n";
static const char ADNEXT_EMPTY[] = 
                "Advance next from empty list!!!\n";
static const char ADPRE_NONEXIST[] = 
                "Advance pre from non-existent list!!!\n";
static const char ADPRE_EMPTY[] = 
                "Advance pre from empty list!!!\n";
static const char DELETE_NONEXIST[] =
                "Deleting from non-existent list!!!\n";
static const char DELETE_NONEXISTNODE[] =
                "Deleting a non-existent node!!!\n";
static const char ISEMPTY_NONEXIST[] =
                "Is empty check from non-existent list!!!\n";
static const char INSERT_NONEXIST[] =
                "Inserting to a non-existent list!!!\n";
static const char REMOVE_NONEXIST[] =
                "Removing from non-existent list!!!\n";
static const char REMOVE_EMPTY[] =
                "Remove from empty list!!!\n";
static const char VIEW_NONEXIST[] = 
                "Viewing a non-existent list!!!\n";
static const char VIEW_NONEXISTNODE[] = 
                "Viewing a non-existent node!!!\n";
static const char VIEW_EMPTY[] =
                "Viewing an empty list!!!\n";
static const char WRITE_NONEXISTFILE[] =
                "Writing to a non-existent file!!!\n";
static const char WRITE_NONEXISTLIST[] =
                "Writing from a non-existent list!!!\n";
static const char WRITE_MISSINGFUNC[] =
                "Don't know how to write out elements!!!\n";
static const char WRITE_NONEXISTNODE[] =
                "Writing from a non-existent node!!!\n";

/* debug messages */
static const char ADNEXT[] = "[List %ld - Advancing next]\n";
static const char ADPRE[] = "[List %ld - Advancing pre]\n";
static const char INSERT[] = "[List %ld - Inserting node]\n";
static const char REMOVE[] = "[List %ld - Removing node]\n";
static const char VIEW[] = "[List %ld - Viewing node]\n";
static const char LIST_ALLOCATE[] = "[List %ld has been allocated]\n";
static const char LIST_DEALLOCATE[] = "[List %ld has been deallocated]\n";

static int debug_on = FALSE;    /* allocation of debug flag */
static long list_counter = 0;   /* the number of lists allocated so far */

/*-------------------------------------------------------------------------- 
Function Name:          set_debug_on
Purpose:                sets debug to true
Description:            sets debug to true 
Input:                  nothing 
Result:                 set debug to true 
--------------------------------------------------------------------------*/
void set_debug_on (void) {
  debug_on = TRUE;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          set_debug_off                                                                                                                    
Purpose:                sets debug to false                                                                            
Description:            sets debug to false                                                                                                              
Input:                  nothing    
Result:                 set debug to false                                                                                                     
--------------------------------------------------------------------------*/
void set_debug_off (void) {
  debug_on = FALSE;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          advance_next_List                                                                                                   
Purpose:                front pointer of this_list moves forward by one Node                                                             
Description:            Checks if This_list or the front is Null. Then
                        checks if there is one item in the list.If it passes
			the front pointer of the list is changed to the next
                        node on the list.                                                                                                      
Input:                  Pointer to This_list
Result:                 shifts the elements of the list forward by one
--------------------------------------------------------------------------*/
void advance_next_List (List * this_list) {
  /*checks for debug*/
  if(debug_on){
    fprintf(stderr, ADNEXT, this_list->list_count);
  }

  /*checks if this_list is NULL*/
  if(this_list == NULL){
    fprintf(stderr,ADNEXT_NONEXIST);
    return;
  }
  /*checks if this list is empty*/
  if(this_list->front == NULL){
    fprintf(stderr, ADNEXT_EMPTY);
    return;
  }

  /*checks if only one item in list*/
  else if(this_list->front->next == NULL){
    return;
  }

  /*changes the front pointer*/
  this_list->front = this_list->front->next;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          advance_pre_List
Purpose:                front pointer of this_list moves backward by one Node                                                       
Description:            Checks if This_list or the front is Null. then
                        checks if there is one item in the list. If it pass
			then it changes the front pointer to the previous node.                                                                
Input:                  Pointer to this_list                                                                           
Result:                 shifts the elements of the list backwards by one                                                                       
--------------------------------------------------------------------------*/
void advance_pre_List (List * this_list) {
  /*checks for debug*/
  if(debug_on){
    fprintf(stderr, ADPRE, this_list->list_count);
  }

  /*checks if this_list is NULL*/
  if(this_list == NULL){
    fprintf(stderr, ADPRE_NONEXIST);
    return;
  }
  /*checks if this list is empty*/
  if(this_list->front == NULL){
    fprintf(stderr, ADPRE_EMPTY);
    return;
  }

  /*checks if only one item in list*/
  else if(this_list->front->next == NULL){
    return;
  }

  /*changes the front pointer*/
  this_list->front = this_list->front->pre;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          delete_list                                                                                  
Purpose:                deletes list and frees it from HEAP                                                                                               
Description:            Checks for if lpp is NULL or points to null and then
                        if it passes it frees the memory and sets the
			pointer to NULL.
Input:                  pointer to the list memory                                                                                                  
Result:                 frees HEAP memory                                                                                                                  
--------------------------------------------------------------------------*/
void delete_List (List ** lpp) {
  Node * node;
  List * lp = *lpp;

  /*Checks if the pointer is null*/
  if(*lpp == NULL){
    fprintf(stderr, DELETE_NONEXIST);
    return;
  }
  
  /*Checks  if the pointer's pointer is null.*/
  else if(lpp == NULL){
    fprintf(stderr, DELETE_NONEXIST);
    return;
  }
  
  /*deletes all the nodes*/
  node = lp->front;
  while(lp->occupancy > 0){
    node = lp->front->next; 
    delete_Node(&lp->front, lp->delete_func);
    lp->front = node;
    --lp->occupancy;
  }
  

  /*Frees the memory of stack and sets the pointer to null*/
  free(*lpp);
  *lpp = NULL;

  /*debug Check*/
  if(debug_on){
    fprintf(stderr, LIST_DEALLOCATE, list_counter);
  }
  --list_counter;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          insert                                                                                                            
Purpose:                Call insert_node and insert a node on the
                        list according to the value of where                                                                                                
Description:            Checks for debug then checks if list in NULL and
                        prints errors. It then checks if sort was pressed
			and calls locate if so. Then it calls insert_node 
			and if where is 1 then makes the front pointer
			the new node. Lastly it adds one to occupancy and
			returns success.                                      
Input:                  Pointer to this_list, element to insert and value
                        of where to insert.                                                                                                     
Result:                 Inserts the new_node accordingly and returns success
                        or error.                                                                                                                
--------------------------------------------------------------------------*/
long insert (List * this_list, void * element, long where) {
  Node *this_Node;
  Node *node;
  long num = 0;

  /*checks for debug*/
  if(debug_on){
    fprintf(stderr, INSERT, this_list->list_count);
  }

  /*checks if this_liost is null*/
  if(this_list == NULL){
    fprintf(stderr, INSERT_NONEXIST);
    return 0;
  }
  
  /*checks if where is 2 if so calls locate to see where to insert the Node*/
  if((where == 2) && (this_list->front != NULL)) {
    this_Node = locate(this_list, element, &num);
  }
  
  /*else make this_Node have the value of the front of the list*/
  else{
    this_Node = this_list->front;
  }
  
  /*calls insert_node and sets return value to node*/
  node = insert_Node(this_Node, element, this_list->copy_func);
  
  /*if where is 1 or list is empty make the front node of the list
    have the same values of node*/
  if((where == 1) || (this_list->front == NULL)) {
    this_list->front = node;
  }
  
  /*checks if where is 2 and if the new node was inserted at the start of
    the list and if so makes the new node the front of the list*/
  else if((num == 9) && (this_Node == this_list->front)){
    this_list->front = node;
  }

  /*adds one to occupancy and return success*/
  ++this_list->occupancy;
  return 1;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          locate                                                                                                                          
Purpose:                locate where to insert the node               
Description:            Compares the value of element to see if it is 
                        smaller or equal to the front node and return the 
			front node if true. If not then it continues on to
			the next node.                                                                                                        
Input:                  Pointer to list and element to insert                                                                                              
Result:                 Returns the index where to insert                                                                                                
--------------------------------------------------------------------------*/
static Node * locate (List * this_list, void * element, long * num) {
  Node *node = this_list->front;
  /*uses a loop to check till what node is element smaller or equal to and
    returns that node*/
  while(1){
    
    /*checks if there is one node on list and returns the node but if
     needing to insert node at front then where is set to 1*/
    if(node->next == NULL){
      if(*(long*)element <= *(long*)node->data){
	*num = 9;
	return node;
      }
      else{
	return node;
      }
    }
	
    /*checks if element is smaller or equal to the front node's data.
     If so it initialzed where to 1 so it inserts at the front and returns 
    the node*/
    if(*(long*)element <= *(long*)this_list->front->data){
      *num = 9;
      return node;
    }

    /*checks if the element is smaller or equal to the previous node's data
     and returns the node*/    
    else if(*(long*)element <= *(long*)node->data){
      return node;
    }

    /*reinitializes node*/
    else{
      /*checks if ti hit the end of the list*/
      if(node->next == this_list->front){
	return node->next;
      }
      node = node->next;
    }
  }
}
 
/*--------------------------------------------------------------------------                                                                                 
Function Name:          isempty_List                                                                                                   
Purpose:                Checks to see if this_list is empty                                                                                 
Description:            Checks if the font pointer of the list is empty.
                        If so that means the list is empty.
Input:                  Pointer to the list                                                                                                      
Result:                 returns success if list is empty and vice verse.                                                 
--------------------------------------------------------------------------*/
long isempty_List (List * this_list) {
  /*checks if this_list is NULL*/
  if(this_list == NULL){
    fprintf(stderr, ISEMPTY_NONEXIST);
  }

  /*checks if the pront pointer is null*/
  if(this_list->front == NULL){
    return 1;
  }
  return 0;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          new_List                                                                                                      
Purpose:                Allocates new HEAP memory for a new list                                                                               
Description:            First it allocates new Heap memory for list. Then
                        it initializes all the values for the list and 
			returns it.
Input:                  function pointers to copy_func, delete_func,
                        write_func and is_greater_than_func.                                                                                    
Result:                 returns pointer to a new list                                                                                                 
--------------------------------------------------------------------------*/
List * new_List (
        void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) {

  /*Allocates memory*/
  List *this_list = (List *) malloc (sizeof (List));
  
  ++list_counter;
  
  /*debug check*/
  if(debug_on){
    fprintf(stderr, LIST_ALLOCATE, list_counter);
  }

  /*initializes the memory*/
  this_list->occupancy = 0;
  this_list->front = NULL;
  this_list->list_count = list_counter;
  this_list->copy_func = copy_func;
  this_list->delete_func = delete_func;
  this_list->is_greater_than_func = is_greater_than_func;
  this_list->write_func = write_func;

  return this_list;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          remove_List
Purpose:                Remove a node from the list                                                                                   
Description:            First checks for debug and this checks if this_list
                        is null. Then it checks is this_list is empty and
			then initialized node according to the value of
			where. Then it deletes the node.
Input:                  Pointer to this_list and where to remove the node                                                              
Result:                 Removes the node from list                                                                                    
--------------------------------------------------------------------------*/
void * remove_List (List * this_list, long where) {
  Node * node;
  void * element;
  /*checks for debug*/
  if(debug_on){
    fprintf(stderr, REMOVE, this_list->list_count);
  }

  /*checks if this_list is null*/
  if(this_list == NULL){
    fprintf(stderr, REMOVE_NONEXIST);
    return 0;
  }

  /*checks if list is empty*/
  else if(isempty_List(this_list)){
    fprintf(stderr, REMOVE_EMPTY);
    return 0;
  }

  /*checks what value where is and initializes node accordinly*/
  if(where == 1){
    node = this_list->front;
    this_list->front = this_list->front->next;
  }
  else{
    /*checks if list has one item*/
    if(this_list->front->next == NULL){
      node = this_list->front;
    }
    else{
      node = this_list->front->pre;
    }
  }

  /*remeoves an occupancy from list*/
  --this_list->occupancy;

  /*checks if only one item in list and if so returns that element and deletes he node*/
  if((this_list->front == NULL) || (this_list->front->next == NULL)){
    element = this_list->front->data;
    delete_Node(&node, this_list->delete_func);
    this_list->front = NULL;
    return element;
  }
  
  /*else call remove_Node, delete the node and then returns element*/
  else{      
    element = remove_Node(node);
    delete_Node(&node, this_list->delete_func);
    return element;
  }
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          view                                                                                                       
Purpose:                view the front or end element of the list                                                                    
Description:            First it checks for debug and then checks if this_list
                        is null. Then it checks if the list is empty and then
			checks the value of where and sets node accordingly.
			Lastly it checks if the node is null and then
			returns the data of the node.
Input:                  Pointer to the list and value of where                                                                     
Result:                 returns element according to where                                  
--------------------------------------------------------------------------*/
void * view (List * this_list, long where) {
  Node * node;
  /*checks for debug*/
  if(debug_on){
    fprintf(stderr, VIEW, this_list->list_count);
  }

  /*checks if this_list is null*/
  if(this_list == NULL){
    fprintf(stderr, VIEW_NONEXIST);
    return 0;
  }
  
  /*checks if list is empty*/
  else if(isempty_List(this_list)){
    fprintf(stderr, VIEW_EMPTY);
    return 0;
  } 

  /*checks what value where is and initializes node accordinly*/
  if(where == 1){
    node = this_list->front;
    
  }
  else{
    /*checks if list has one item*/
    if(this_list->front->next == NULL){
      node = this_list->front;
    }
    else{
      node = this_list->front->pre;
    }
  }

  /*checks if node is none existant*/
  if(node == NULL){
    fprintf(stderr, VIEW_NONEXISTNODE);
  }
  
  /*return the element at that node*/
  return view_Node(node);
}

FILE * write_List (List * this_list, FILE * stream) {
        long count;             /* to know how many elements to print */
        Node * working;                 /* working node */

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_list) {
                fprintf (stderr, WRITE_NONEXISTLIST);
                return NULL;
        }

        if (stream == stderr)
                fprintf (stream, "List %ld has %ld items in it.\n",
                        this_list->list_count, this_list->occupancy);

        if (!this_list->write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        if (this_list->occupancy >= 1)
                working = this_list->front;

        for (count = 1; count <= this_list->occupancy; count++) {
                if (stream == stderr)
                        fprintf (stream, "\nelement %ld:  ", count);
                write_Node (working, stream, this_list->write_func);
                working = working->next;
        }
                
        return stream;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          write_reverse_list                                                          
Purpose:                To write out the elements of the list in reverse
                        order                                                                                                
Description:            First it checks to see if this_list or stream is 
                        null. Then prints out the list and how many nodes
			it has. Then it checks to see that the write func
			is on this_list. 
Input:                  Pointer to this_list and pointer to stream                                                                            
Result:                 returns a pointer to stream with the order reversed                                                                      
--------------------------------------------------------------------------*/
FILE * write_reverse_List (List * this_list, FILE * stream) {
  long count;             
  Node * working;         
   
  /*checks if this_list is null*/
  if (this_list == NULL){
    fprintf(stderr, WRITE_NONEXISTLIST);
  }

  /*checks if file is null*/
  else if (stream == NULL){
    fprintf(stderr, WRITE_NONEXISTFILE);
  }
  
  /*prints the list and occupancy*/
  if (stream == stderr){
    fprintf (stream, "List %ld has %ld items in it.\n", this_list->list_count, this_list->occupancy);
  }

  /*Checks if the write_func is null*/
  if (this_list->write_func == NULL) {
    fprintf (stream, WRITE_MISSINGFUNC);
    return stream;
  }

  /*checks if there is only one node in list and prints it*/
  if(this_list->front->pre == NULL){
    working = this_list->front;
    
    /*prints the element count*/
    if (stream == stderr){
      fprintf (stream, "\nelement %ld:  ", this_list->occupancy);
    }
    
    /*prints the node and returns stream*/
    write_Node (working, stream, this_list->write_func);
    return stream;
  }

  /*sets working to the last node*/
  if (this_list->occupancy >= 1){
    working = this_list->front->pre;
  }

  /*changes working to decrease node count*/
  for (count = this_list->occupancy; count > 0; --count) {

    /*prints the element count*/
    if (stream == stderr){
      fprintf (stream, "\nelement %ld:  ", count);
    }

    /*prints the node and updates working*/
    write_Node (working, stream, this_list->write_func);
    working = working->pre;
  }

  return stream;
}

static void delete_Node (Node ** npp, void (*delete_func) (void *)) {

        /* does the node exist??? */
        if (!npp || !*npp) {
                fprintf (stderr, DELETE_NONEXISTNODE);
                return;
        }

        /* call function to delete element */
        if (delete_func && (*npp)->data)
                (*delete_func) (&((*npp)->data));

        /* delete element */
        free (*npp);

        /* assign node to NULL */
        *npp = NULL;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          insert_Node                           
Purpose:                To create a new node an insert it in the list  
Description:            Checks for copy_func then creates the new_node. It
                        Then if the list is empty then insert the node and
			return. If there is one thing in the list then 
			insert it. else it sert the new_node accordingly.
Input:                  the node at the front of the list, the element
                        we want inserted and copy_func.                                                                                                  
Result:                 returns Pointer to the new_node created and inserts
                        it
--------------------------------------------------------------------------*/
static Node * insert_Node (Node * this_Node, void * element, 
        void * (*copy_func) (void *)) {
  Node *new_node;
  /*checks if copy_func is NULL and created a new_node*/
  if(copy_func == NULL){
    new_node = new_Node(element, copy_func);
  }

  else{
    new_node = new_Node(element, copy_func);
  }

  /*checks if list is empty and inserts new_node*/
  if(this_Node == NULL){
    new_node->pre = NULL;
    new_node->next = NULL;
    return new_node;
  }
  
  /*checks if only one node on list and inserts new_node while changing the pre and next*/
  else if((this_Node->pre == NULL) && (this_Node->next == NULL)){
    this_Node->pre = this_Node->next = new_node;
    new_node->pre = new_node->next = this_Node;
    return new_node;
  }
     
  /*insert the new_node and changes the other nodes accordingly*/
  else{
    new_node->pre = this_Node->pre;
    new_node->next = this_Node;
    this_Node->pre->next = new_node;
    this_Node->pre = new_node;
    return new_node;
  }

    
}

static Node* new_Node (void * element, void * (*copy_func) (void *)) {

        /* allocate memory */
        Node *this_Node = (Node *) malloc (sizeof (Node));

        /* initialize memory */
        this_Node->next = this_Node->pre = NULL;
        this_Node->data = (copy_func) ? (*copy_func) (element) : element;

        return this_Node;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          remove_Node                                                                                                  
Purpose:                Removes this_node from the list                         
Description:            Changes the values of pre and next accordingly to
                        remove the node and then returns the data of
			this_node.
Input:                  Pointer to this_Node                                                                     
Result:                 Return the element from the node that was removed                                                                        
--------------------------------------------------------------------------*/
static void * remove_Node (Node * this_Node) {
  Node * next_Node = this_Node->next;
  this_Node->next->pre = this_Node->pre;
  this_Node->pre->next = next_Node;
  return this_Node->data;
}

/*--------------------------------------------------------------------------                                                                                 
Function Name:          view_Node                                                                                                     
Purpose:                Return the element from node                                                                                             
Description:            This_Node is already non-null so then it returns 
                        the element at that node.                                                         
Input:                  Pointer to this_Node                                                                                          
Result:                 returns element which the this_Node holds                                                                            
--------------------------------------------------------------------------*/
static void * view_Node (Node * this_Node) {
  return this_Node->data;
}

static FILE* write_Node (Node * this_Node, FILE * stream,
        FILE * (*write_func) (void *, FILE *)) {

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_Node) {
                fprintf (stream, WRITE_NONEXISTNODE);
                return stream;
        }

        if (!write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        return (*write_func) (this_Node->data, stream);
}
/*Debug questions:

1. 0x604070
2. 0x604050
3. 0x6040f0
4. 0x6040b0
5. 0x604090
6. 0x604070
7. 0x6040f0
8. 0x6040d0
9. 0x6040b0
10. 0x604070 

*/
