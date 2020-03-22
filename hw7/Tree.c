/****************************************************************************

                                                Aaron Hernandez
                                                CSE 12, Spring 2017
                                                May 17, 2017
                                                cs12xcb
                              Assignment 7

File Name:      Tree.c
Description:    Creates a binary tree data stucture to store UCSDStudents
                or Variables objects. Can turn on or off debug and can also
                insert, lookup and remove nodes which are the objects.

****************************************************************************/
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "Tree.h"
using namespace std;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static const char ALLOCATE[] = " - Allocating]\n";
static const char DEALLOCATE[] = " has been deallocated]\n";
static const char AND[] = " and ";
static const char COMPARE[] = " - Comparing ";
static const char INSERT[] = " - Inserting ";
static const char TREE[] = "[Tree ";

//=========================================================================
// struct Node
//
// Description: Implements the node of the binary search tree data structure.
//     Each node contains two children, the left child which contains
//     data "less than" the data of the current node, and the right child
//     which contains data "greater than" the data of the current node.
//
// Data Fields:
//     data (Base *)  - holds the data stored in the current node
//     left (Node *)  - the left child
//     right (Node *) - the right child
//     parent (Node*) - the parent
//     occupancy (long) - how many TNodes there is in the tree
//     hasBeenDeleted (long) - true or false if whether it has been deleted
//     balance (long) - left child's height - right child's height
//     height (long) - 1 + height of tallest child, or 0 for leaf
//
// Public functions:
//     Tree - Constructor
//     ~Tree - Descructor
//     delete_AllNodes - deletes all the children of the current node
//     Write_AllNodes - display the current node and all its children
//     Write - print a nodes data field
//==========================================================================
struct TNode {
  Base * data;
  TNode * left, * right, *parent;
  static long occupancy;
  unsigned long hasBeenDeleted;
  
  // left child's height - right child's height
  long balance;
  
  // 1 + height of tallest child, or 0 for leaf
  long height;

  //TNode Constructor
  TNode (Base * element) : data (element), left (0), right (0),
    parent (0), hasBeenDeleted (FALSE), balance (0), height(0) {
  }

  //TNode Deconstuctor
  ~TNode (void) {
    delete (data);
    
  }

  ostream & Write (ostream & stream) const {
    stream << "at height:  " << height << "  with balance:  "
	   << balance << "  ";
    return data->Write (stream) << "\n";
  }

  ostream & Write_AllTNodes (ostream & stream) const{
    if (left){
      left->Write_AllTNodes (stream);
    }
    if (!hasBeenDeleted){
      Write (stream);
    }
    if (right){
      right->Write_AllTNodes (stream);
    }
    return stream;
  }
  
  //Dealloctes all memory for TNode
  void delete_AllTNodes (void) {

    //go left
    if (left)
      left->delete_AllTNodes();

    //go right
    if (right)
      right->delete_AllTNodes();

    //Call deconstuctor for element   
    delete (this);
  }
};

// initialization of static data fields of Tree
long TNode :: occupancy = 0;
bool Tree :: debug_on = 0;

void Tree :: Set_Debug (bool option)
/*---------------------------------------------------------------------------
Function Name:                Set_Debug
Purpose:                      This function exists to set debug
Description:                  This function initializes debug
Input:                        option:  what debug gets set to
Output:                       None.
Result:                       Debug is set
---------------------------------------------------------------------------*/
{
  debug_on = option;
}


Tree :: Tree (const char * name) : root (0), tree_name (strdup (name))
/***************************************************************************
% Routine Name : Tree :: Tree  (public)
% File :         Tree.c
% 
% Description :  Initializes root pointer to NULL, and occupancy to 0.
***************************************************************************/
{
  TNode::occupancy = 0;
  //Debug Check
  if(debug_on){
    cerr << TREE << tree_name << ALLOCATE;
  }
}// end: Tree


Tree :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/
{
  //Checks debug
  if(debug_on){
    cerr << TREE << tree_name << DEALLOCATE;
  }

  //Calls delete_AllTNode
  if(root){
    root->delete_AllTNodes();
  }

  //frees the tree name memory
  free((void *)tree_name);  
}	/* end: ~Tree */


unsigned long Tree :: Insert (Base * element)
/*---------------------------------------------------------------------------
Function Name:                Insert
Purpose:                      This function inserts an element in a tree
Description:                  First it checks if tree is empty and if so
                              inserts into tree and checks for debug. If it
			      fails it then compares the element against
			      other nodes to find a place to insert it.
			      It then goes back up the tree and adjusts
			      the heights and balances. Once done it returns
			      success or fail.
Input:                        element:  pointer to the element
Output:                       True or false of whether it was inserted or not
Result:                       Element is inserted
---------------------------------------------------------------------------*/
{
  //Cretes a new node with element and current Node
  TNode * node = new TNode(element);
  TNode * currentNode = root;
  
  //If tree is empty simply insert node 
  if (TNode::occupancy == 0){
    root = node;
    ++root->height;
    ++TNode::occupancy;

    //Checks for debug
    if(debug_on){
      cerr << TREE << tree_name << INSERT << *element << "]\n";
    } 

    return true;
  }
  
  //If tree is not empty loop
  else {
    while(true){
      
      //Checks for debug
      if(debug_on){
	cerr << TREE << tree_name << COMPARE << *element << AND << *currentNode->data << "]\n";
      }
      
      //Condition if element needs to go right
      if(!(*element > *currentNode->data)){
	
	//Check if the currentNode's left is empty and
	//if so breaks from loop and integrates element
	if(currentNode->left == NULL){
	  currentNode->left = node;
	  node->parent = currentNode;
	  break;
	}
	
	//If not empty reset currentNode to the left node
	else{
	  currentNode = currentNode->left;
	  continue;
	}
      }
      
      //else the element goes left
      else{
	
	//Check if the currentNode's left is empty and
	//if so breaks from loop and integrates element
	if(currentNode->right == NULL){
	  currentNode->right = node;
	  node->parent = currentNode;
	  break;
	}
	
	//If not empty reset currentNode to the right node
	else{
	  currentNode = currentNode->right;
	  continue;
	}
      }
    } //matches while loop
  }
  currentNode = node->parent; //update currentNode
  
  //Adjusts the Height and Balance
  while(true){
    
    //keeps track of the tallest child of a parent
    // and of left and right childs
    long tallestChild;
    TNode * left = currentNode->left;
    TNode * right = currentNode->right;
    
    long leftH; //left height
    long rightH; //right height
    
    //Checks if there is no left child
    //If so it sets tallest child to the right
    if(left == NULL){
      rightH = tallestChild = right->height;
      leftH = -1;
    }
    
    //Checks if there is no right child
    //If so it sets tallest child to the left
    else if (right == NULL){
      leftH = tallestChild = left->height;
      rightH = -1;
    }
    
    //Else pick the tallest child
    else{
      leftH = left->height;
      rightH = right->height;
      tallestChild = (leftH > rightH) ? leftH : rightH;
    }
    
    //Adjusts the height
    currentNode->height = 1 + tallestChild;
    
    //Adjusts the balance
    currentNode->balance = leftH - rightH;
    
    //Checks if currentNode is root and if so quits
    if(currentNode->data == root->data){
      break;
    }
    
    //Resets the currentNode
    currentNode = currentNode->parent;
    
  }//Matches while loop

  //Checks for debug
  if(debug_on){
    cerr << TREE << tree_name << INSERT << *element << "]\n";
  }
  
  ++TNode::occupancy;
  return true;
} //end: insert


const Base * Tree :: Lookup (const Base * element) const
/*---------------------------------------------------------------------------
Function Name:                Lookup
Purpose:                      Looks up an element to find in tree
Description:                  This function checks if the tree is empty and
                              then checks for debug. Then it checks each
			      element till it finds the one it matches.
			      Once found, it checks hasBeen Deleted and 
			      returns the found data
Input:                        element:  Pointer to the element to find
Output:                       Pointer to the element that is removed
Result:                       Finds an element in the tree
---------------------------------------------------------------------------*/
{
  //Cretes a new node with element and current Node
  TNode * currentNode = root;
  
  //If tree is empty return null 
  if (TNode::occupancy == 0){
    return NULL;
  }	    
  
  //Checks Tree for element
  while(true){
    
    //Checks for debug
    if(debug_on){
      cerr << TREE << tree_name << COMPARE << *element << AND << *currentNode->data << "]\n";
    }
    
    //Checks if element found
    if(*element == *currentNode->data){
      
      //checks if it has been deleted
      if(currentNode->hasBeenDeleted){
	return NULL;
      }
      
      return currentNode->data;
    }
    
    //Condition if element needs to go right
    else if(!(*element > *currentNode->data)){
      
      //Check if the currentNode's left is empty and
      //if so return null
      if(currentNode->left == NULL){
	return NULL;
      }
      
      //If not empty reset currentNode to the left node
      else{
	currentNode = currentNode->left;
      }
    }
    
    //else the element goes left
    else{
      
      //Check if the currentNode's left is empty and
      //if so return null
      if(currentNode->right == NULL){
	return NULL;
      }
      
      //If not empty reset currentNode to the right node
      else{
	currentNode = currentNode->right;
      }
    }
  }//matches while
} //end: lookup


Base * Tree :: Remove (const Base * element)
/*---------------------------------------------------------------------------
Function Name:                Remove
Purpose:                      Lazy remove a TNode from the Tree
Description:                  This function checks if the tree is empty and
                              then checks for debug. Then it checks each
			      element till it finds the one to delete.
			      Once found, it removes one from occupancy,
			      sets hasBeen Deleted to true and return the
			      removed data
Input:                        element:  Pointer to the element to remove
Output:                       Pointer to the element that is removed
Result:                       Removes a TNode from tree
---------------------------------------------------------------------------*/
{
  //Cretes a new node with element and current Node
  TNode * currentNode = root;
  
  //If tree is empty return null 
  if (TNode::occupancy == 0){
    return NULL;
  }	    
  
  //Checks Tree for element
  while(true){
    
    //Checks for debug
    if(debug_on){
      cerr << TREE << tree_name << COMPARE << *element << AND << *currentNode->data << "]\n";
    }
    
    //Checks if element found
    if(*element == *currentNode->data){
      currentNode->hasBeenDeleted = true;
      --TNode::occupancy;
      return currentNode->data;
    }
    
    //Condition if element needs to go right
    else if(!(*element > *currentNode->data)){
      
      //Check if the currentNode's left is empty and
      //if so return null
      if(currentNode->left == NULL){
	return NULL;
      }
      
      //If not empty reset currentNode to the left node
      else{
	currentNode = currentNode->left;	
      }
    }
    
    //else the element goes left
    else{
      
      //Check if the currentNode's left is empty and
      //if so return null
      if(currentNode->right == NULL){
	return NULL;
      }
      
      //If not empty reset currentNode to the right node
      else{
	currentNode = currentNode->right;
      }
    }
  }//matches while
} //end: Remove


ostream & Tree :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
% 
% Description : This function will output the contents of the Tree table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/
{
        stream << "Tree " << tree_name << ":\n"
		<< "occupancy is " << TNode :: occupancy << " elements.\n";

	return (root) ? root->Write_AllTNodes (stream) : stream;
}       /* end: Write */
