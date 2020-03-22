/****************************************************************************

  Aaron Hernandez
  Othman Akel
  CSE 12, Spring 2017
  June 9, 2017
  cs12xcb
  cs12xaa
  Assignment 9

  File Name:      Tree.c

Description:    Creates a binary tree data structure to store UCSDStudents
or Variables objects. Can turn on or off debug and can also
insert, lookup and remove nodes which are the objects.
 ****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "Tree.h"

// debug messages
static const char ALLOCATE[] = " - Allocating]\n";
static const char COST_READ[] = "[Cost Increment (Disk Access): Reading ";
static const char COST_WRITE[] = "[Cost Increment (Disk Access): Writing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char TREE[] = "[Tree ";



template <class Whatever>
int Tree<Whatever>::debug_on = 0;

template <class Whatever>
long Tree<Whatever>::cost = 0;

template <class Whatever>
long Tree<Whatever>::operation = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);

template <class Whatever>
struct  TNode {
	// friends:

	// data fields:
	Whatever data;
	long height;
	long balance;
	offset left;
	offset right;
	offset this_position;	// current position

	// function fields:
	TNode () : height (0), balance (0), left (0), right (0), 
	this_position (0) {}

	// to declare the working TNode in Tree's Remove
	TNode (Whatever & element) : data (element), height (0), balance (0),
	left (0), right (0), this_position (0) {}

	TNode (Whatever &, fstream *, long &);	// to add new node to disk
	TNode (const offset &, fstream *);	// to read node from disk

	unsigned long Insert (Whatever &, fstream *, long &, offset &);
	// optional recursive Lookup declaration would go here
	unsigned long Lookup (Whatever &, fstream *) const;

	void Read (const offset &, fstream *);	// read node from disk
	unsigned long Remove (TNode<Whatever> &, fstream *, long &, offset &,
			long fromSHB = FALSE);
	void ReplaceAndRemoveMax (TNode<Whatever> &, fstream *, offset &);
	void SetHeightAndBalance (fstream *, offset &);
	void Write (fstream *) const;		// update node to disk

	ostream & Write_AllTNodes (ostream &, fstream *) const;
};


/***************************************************************************
  % Routine Name : Tree<Whatever> :: Set_Debug_On (public)
  % File :         Tree.c
  %
  % Description : This function sets debug to 1 or true.
  %
 ***************************************************************************/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_On (void){
	debug_on = 1;
}

/***************************************************************************
  % Routine Name : Tree<Whatever> :: Set_Debug_Off (public)
  % File :         Tree.c
  %
  % Description : This function sets debug to 0 or false.
  %
 ***************************************************************************/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_Off (void){  
	debug_on = 0;
}


/***************************************************************************
  % Routine Name : Tree<Whatever> :: Insert (public)
  % File :         Tree.c
  %
  % Description : This function inserts the element if the tree is empty else
  %               it delegates the insert job to TNode's insert function.
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % element            A reference to the element to be inserted.
  % <return>           true or false whether it was inserted or not.
 ***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (Whatever & element) 
{
	//Call increment operation function
	Tree<Whatever>::IncrementOperation();

	//Creates a new node with element if tree is empty, sets root to it and
	//returns true  
	if(!occupancy){
		TNode <Whatever> rootNode (element, fio, occupancy);
		root = rootNode.this_position;
		return TRUE;                      
	}

	//Grab root Via read constructor
	//Delegate to root’s insert
	TNode <Whatever> node (root, fio);
	return node.Insert(element, fio, occupancy, root);  
}


/***************************************************************************
  % Routine Name : TNode :: ReplaceAndremoveMax  (public)
  % File :         Tree.c
  %
  % Description :  Replaces that TNode with the maximum TNode in its left
  %                subtree to maintain the Tree structure.
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % targetTNode        A reference to the targetTNode.
  % fio                A fstream pointer.
  % PositionInParent   A reference to the PositionInParent
 ***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: ReplaceAndRemoveMax (TNode<Whatever> & targetTNode,
		fstream * fio, offset & PositionInParent) 
{
	//Goes right till it is null
	if (right){
		TNode <Whatever> rightNode(right, fio);
		rightNode.ReplaceAndRemoveMax(targetTNode, fio, right);
	}

	//Once done it checks to see if that node has a left child 
	else {
		//If it has a left child the set PositionInParent to that
		if (left) {
			PositionInParent = left;
		}

		//Else set PositionInParent to null
		else {
			PositionInParent = NULL;
		}

		//Set targetTNode's data to data
		targetTNode.data = data;    
	}

	//calls set height and balance
	SetHeightAndBalance(fio, PositionInParent);
}

/***************************************************************************
  % Routine Name : TNode :: Remove  (public)
  % File :         Tree.c
  %
  % Description :  This searches for the node to remove until found and then
  %                It checks if it is going to remove a node with zero childs,
  %                1 child or 2 childs. Then it does the appropiate steps to
  %                remove the node.
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % elementTNode       A reference to the elementTNode to be removed.
  % fio                A fsteam pointer
  % occupancy          long of the occupancy
  % PositionInParent   A reference to the PositionInParent
  % fromSHB            A long of whether it was called from SHB or not
 ***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Remove (TNode<Whatever> & elementTNode,
		fstream * fio, long & occupancy, offset & PositionInParent,
		long fromSHB) 
{  
	//Checks if element is to the right
	if (elementTNode.data > data) {

		//If element not in tree return false
		if (right == 0) {
			return FALSE;
		}

		//else move down the tree to the right
		else {
			TNode <Whatever> rightNode(right, fio);
			rightNode.Remove(elementTNode, fio, occupancy, 
								right, FALSE);
		}
	}

	//Checks if element was found
	else if (data == elementTNode.data) {

		//If removing a leaf then correctly set elementTNode's data and
		//PositionInParent. Then returns true
		if ((right == 0) && (left == 0)) { 
			elementTNode.data = data;
			PositionInParent = 0;
			return TRUE;
		}

		//If removing a node of that has one child then correctly
		// update PositionInParent
		else if ((right == 0) ^ (left == 0)) {

			//If the one child was on the right
			if (right){
				PositionInParent = right;	
			}
			//Else the one child is on the left
			else{
				PositionInParent = left;
			}

			//Sets elemsntTNode's data to data
			elementTNode.data = data;
		}

		//two child node remove and call RARM
		else{
			TNode <Whatever> leftNode(left, fio);
			leftNode.ReplaceAndRemoveMax(*this,fio, left);
			Write(fio);
		}
	}

	//Else element is to the left
	else {

		//If element not in tree return false
		if (left == 0) {
			return FALSE;
		}

		//else move down the tree to the left
		else {
			TNode <Whatever> leftNode(left, fio);
			leftNode.Remove(elementTNode, fio, occupancy,
					left, FALSE);
		}
	}

	//Calls SHB if not called from SHB
	if (!fromSHB){
		SetHeightAndBalance(fio, PositionInParent);
	}

	//Returns true          
	return TRUE;
}


/***************************************************************************
  % Routine Name : Tree<Whatever> :: Remove (public)
  % File :         Tree.c
  %
  % Description : This function returns false if the tree is empty otherwise
  %               delegates the remove job to TNode's remove function. If the
  %               root was removes then it calls ResetRoot.
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % element            A reference to the element to be removed.
  % <return>           true or false whether it was removed or not.
 ***************************************************************************/
	template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) 
{
	//holds return value
	long retval; 

	//Checks if its an empty tree
	if (!occupancy) {
		return FALSE;
	}

	//Call increment operation function
	Tree<Whatever>::IncrementOperation();

	//If not then delegate it to TNode's remove 
	TNode <Whatever> elementNode (element);
	TNode <Whatever> rootNode (root, fio);
	retval = rootNode.Remove(elementNode, fio, occupancy, root, FALSE);

	//Sets element to the found elementTNode and decrements occupancy
	element = elementNode.data;
	occupancy--;

	//Checks if occupancy and if so calls ResetRoot
	if(!occupancy){
		ResetRoot();
	}

	return retval;	
}


/***************************************************************************
  % Routine Name : TNode :: SetHeightAndBalance  (public)
  % File :         Tree.c
  %
  % Description :  Updates the height and balance of the current TNode.
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % fio                A fstream pointer
  % PointerInParent    A reference to the PositionInParent
 ***************************************************************************/
	template <class Whatever>
void TNode<Whatever> :: SetHeightAndBalance (fstream * fio,
		offset & PositionInParent) 
{
	//Holds the heights of no childs
	long left_height = -1;
	long right_height = -1;

	//If there is a left childe then set right_Height correctly
	if (left) {
		TNode <Whatever> leftNode(left, fio);
		left_height = leftNode.height;
	}

	//If there is a left childe then set right_Height correctly
	if (right) {
		TNode <Whatever> rightNode(right, fio);
		right_height = rightNode.height;
	}

	//Sets the height
	height = ((right_height > left_height)?
			right_height + 1: left_height +1);

	//Sets the balance
	balance = left_height - right_height;



	//Threshold check
	if (abs(balance) > THRESHOLD) {
		long fakeOccupancy = 0; //fake occupancy

		//Removes the node and then reinserts it
		TNode <Whatever> node(data);  //saves data
		Remove(*this, fio, fakeOccupancy, PositionInParent, TRUE);
		TNode <Whatever> temp(PositionInParent, fio); //new TNode with PIP
		temp.Insert(node.data, fio, fakeOccupancy, PositionInParent);
	}

	//call write function
	else 
		Write(fio);

}


/***************************************************************************
  % Routine Name : Tree<Whatever> :: GetCost (public)
  % File :         Tree.c
  %
  % Description : Returns the cost long
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % <return>          returns a long of the cost 
 ***************************************************************************/
	template <class Whatever>
long Tree <Whatever> :: GetCost () 
{
	//return cost
	return cost;
}


/***************************************************************************
  % Routine Name : Tree<Whatever> :: GetOperation (public)
  % File :         Tree.c
  %
  % Description : Returns the operation long
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % <return>           returns a long of the operation
 ***************************************************************************/
	template <class Whatever>
long Tree <Whatever> :: GetOperation () 
{
	//return operation
	return operation;
}


/***************************************************************************
  % Routine Name : Tree<Whatever> :: IncrementCost (public)
  % File :         Tree.c
  %
  % Description : Increments cost by one.
 ***************************************************************************/
	template <class Whatever>
void Tree <Whatever> :: IncrementCost () 
{
	//increments cost
	++cost; 
}


/***************************************************************************
  % Routine Name : Tree<Whatever> :: IncrementOperation (public)
  % File :         Tree.c
  %
  % Description : Increments operation by one.
 ***************************************************************************/
	template <class Whatever>
void Tree <Whatever> :: IncrementOperation () 
{
	//increments operation
	++operation; 
}



/***************************************************************************
  % Routine Name : Tree<Whatever> :: ResetRoot (public)
  % File :         Tree.c
  %
  % Description : Resets the root datafield of this tree to be at the end of
  %               the datafile
 ***************************************************************************/
	template <class Whatever>
void Tree <Whatever> :: ResetRoot () 
{
	//Sets the pointer p to the end and makes root equal to that
	fio->seekp(0, ios::end);
	root = fio->tellp();
}


/***************************************************************************
  % Routine Name : TNode<Whatever> :: Insert (public)
  % File :         Tree.c
  %
  % Description : This function checks for debug and then sees if the element
  %               should go right or left till it finds an empty sopt on the
  %               tree and inserts it.
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % element            A reference to the element to be inserted.
  % PointerInParent    Pointer reference to the parent TNode
  % <return>           true or false whether it was inserted or not.
 ***************************************************************************/
	template <class Whatever>
unsigned long TNode<Whatever> :: Insert (Whatever & element, fstream * fio,
		long & occupancy, offset & PositionInParent) 
{
	//Condition if element needs to go right                                
	if(element > data){

		//Check if the PositionInParent's right is empty and                        
		//if so integrates element                       
		if(!right){
			TNode <Whatever> newNode (element, fio, occupancy);
			right = newNode.this_position;
		}

		//If not call insert again with updated PositionInParent
		else{
			TNode <Whatever> rightNode (right, fio);
			rightNode.Insert(element, fio, occupancy, right);
		}
	}

	//if the element is found set element to data
	else if (element == data) {
		data = element;
	}

	//else the element goes left                                            
	else{

		//Check if the PositionInPerent's left is empty and                        
		//if so integrates element                       
		if(!left){
			TNode <Whatever> newNode (element, fio, occupancy);
			left = newNode.this_position;    
		}

		//If not call insert again with updated PositionInParent                    
		else{
			TNode <Whatever> leftNode(left, fio);
			leftNode.Insert(element, fio, occupancy, left);
		}
	}

	//Calls set height and balance
	SetHeightAndBalance(fio, PositionInParent);

	//return true 
	return true;
}

/***************************************************************************
  % Routine Name : TNode<Whatever> :: Lookup (public)
  % File :         Tree.c
  %
  % Description : This function checks to see if the tree is empty and if not
  %               delegates it to TNode's lookup
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % element            A reference to the element to be lookedup.
  % <return>           true or false of whether it was found or not.
 ***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Lookup(Whatever & element,
		fstream * fio) const
{
	long retval; //hold return value

	//checks if element goes right
	if (element > data) {

		//if element is not in tree return false
		if (!right) {
			return FALSE;
		}

		//else move down the tree to the right
		else {
			TNode <Whatever> rightNode(right, fio);
			retval = rightNode.Lookup(element, fio);
		}
	}

	//if element found set element to data and return true
	else if (element == data) {
		element = data;
		return TRUE;
	}    

	//Else go left
	else {

		//if element is not in tree return false
		if (!left) {
			return FALSE;
		}

		//else move down the tree to the left
		else {
			TNode <Whatever> leftNode(left, fio);
			retval = leftNode.Lookup(element, fio);
		}     
	}  

	//return retval
	return retval;
}


/***************************************************************************
  % Routine Name : Tree<Whatever> :: Lookup (public)
  % File :         Tree.c
  %
  % Description : This function checks to see if the tree is empty and if not
  %               delegates it to TNode's lookup
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % element            A reference to the element to be lookedup.
  % <return>           true or false of whether it was found or not.
 ***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const
{ 
	//empty tree
	if (!occupancy) {
		return FALSE;
	}

	//Call increment operation function
	Tree<Whatever>::IncrementOperation();

	//regular lookup
	TNode <Whatever> rootNode (root, fio);
	return rootNode.Lookup(element, fio);  	
}





/***************************************************************************
  % Routine Name : TNode<Whatever> :: Read (public)
  % File :         Tree.c
  %
  % Description : Sets the g pointer to position and reads the TNode. Then it
  %               checks for debug and calls the increment cost function.
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % position            A reference to offset position.
  % fio                 An fstream pointer
 ***************************************************************************/
	template <class Whatever>
void TNode<Whatever> :: Read (const offset & position, fstream * fio) 
{
	//Sets the p pointer to the positions and reads from disk
	fio->seekg(position);
	fio->read((char *) this, sizeof(*this));

	//Debug check
	if(Tree <Whatever>::debug_on){
		cerr << COST_READ << (const char *)this << "]\n";
	}

	//Calls the increment cost function
	Tree<Whatever>::IncrementCost();
}

/***************************************************************************
  % Routine Name : TNode :: TNode (Read constuctor) (public)
  % File :         Tree.c
  %
  % Description :  Calls the read function.
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % position            A reference to offset position.
  % fio                 An fstream pointer
 ***************************************************************************/
	template <class Whatever>
TNode<Whatever> :: TNode (const offset & position, fstream * fio) 
{
	//Calls the Read function
	Read(position, fio);
}

/***************************************************************************
  % Routine Name : TNode :: TNode (Write constuctor) (public)
  % File :         Tree.c
  %
  % Description :  Guarantee initialization of data, height, balance, left,
  %                right. It also adds one to occupancy and seeks the pointer 
  %                p to the end of the file and sets this_position to that.
  %                Then it calls the write function.
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % element            A reference to the element to be inserted.
  % theTree            A reference to the Tree
 ***************************************************************************/
template <class Whatever>
TNode<Whatever> :: TNode (Whatever & element, fstream * fio,long & occupancy): 
	data (element), height (0), balance (0), left (0), 
	right (0) 
{
	//Adds 1 to occupancy, sets the p pointer to the end and makes this_position
	// equal to that. Then it calls the Write function
	++occupancy; 
	fio->seekp(0, ios::end);
	this_position = fio->tellp();
	Write(fio);
}


/***************************************************************************
  % Routine Name : TNode<Whatever> :: Write (public)
  % File :         Tree.c
  %
  % Description : Puts the p pointer to the position of the TNode, writes the
  %               TNode to the fileand then calls the increment cost function.
  %
  % Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % fio               A fstream pointer
 ***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: Write (fstream * fio) const 
{
	//setts the p pointer to this_postition and writes it to the disk
	fio->seekp(this_position);
	fio->write((const char *)this, sizeof(*this));

	//Debug check
	if(Tree <Whatever>::debug_on){
		cerr << COST_WRITE << (const char *)this << "]\n";
	}

	//Call increment cost function
	Tree<Whatever>::IncrementCost();
}


/***************************************************************************
  % Routine Name : Tree<Whatever> :: Tree  (public)
  % File :         Tree.c
  %
  % Description :  Guarantee initialization of occupancy and root. It also
  %                initializes the tree_count using a static counter.
  %
  %  Parameters descriptions :
  %
  % name               description
  % -------------------------------------------------------------------------
  % datafile            A const char * to the datafile that contains info for
  %                     tree
 ***************************************************************************/
	template <class Whatever>
Tree<Whatever> :: Tree (const char * datafile) : fio (new fstream (datafile,
			ios :: out | ios :: in))
{
	//For tree number
	static long counter;
	tree_count = ++counter;

	//Get end of data file
	fio  -> seekp (0, ios::end);
	offset ending = fio -> tellp();

	//Get beginning of data file
	fio -> seekp(0, ios::beg);
	offset beginning = fio ->tellp();

	// empty datafile
	if (beginning == ending) {

		//Writes the root to empty data file
		root = 0;
		fio->write ((const char *) &root, sizeof (root));

		//Writes occupancy to empty data file 
		occupancy = 0;
		fio -> write ((const char *)&occupancy, sizeof(occupancy));

		//Saves root
		root = fio->tellp();
	}

	// non empty datafile case
	else {

		//Reads in the root and the occupancy
		fio -> seekg(0, ios::beg);
		fio -> read ((char *) &root, sizeof (root));
		fio -> read ((char *) &occupancy, sizeof (occupancy));
	}

	//Debug Check
	if(debug_on)
	{
		cerr << TREE << tree_count << ALLOCATE;
	}
}


/***************************************************************************
  % Routine Name : Tree :: ~Tree  (public)
  % File :         Tree.c
  % 
  % Description :  Checks for debug then writes the updated root and occupancy
  %               to the disk.
 ***************************************************************************/
	template <class Whatever>
Tree<Whatever> :: ~Tree (void)
{
	//Checks debug
	if(debug_on)
	{
		cerr << TREE << tree_count << DEALLOCATE;
	}

	//Writes root and occupancy to disk
	fio -> seekp(0, ios::beg);
	fio -> write ((const char *) &root, sizeof (root));
	fio -> write ((const char *) &occupancy, sizeof(occupancy));
	delete (fio);
}	/* end: ~Tree */





	template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) 
{
	stream << "at height:  :" << nnn.height << " with balance:  "
		<< nnn.balance << "  ";
	return stream << nnn.data << "\n";
}



/***************************************************************************
  % Routine Name : Tree :: Write (public)
  % File :         Tree.c
  % 
  % Description : This funtion will output the contents of the Tree table
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
template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
{
	long old_cost = cost;

	stream << "Tree " << tree_count << ":\n"
		<< "occupancy is " << occupancy << " elements.\n";

	fio->seekg (0, ios :: end);
	offset end = fio->tellg ();

	// check for new file
	if (root != end) {
		TNode<Whatever> readRootNode (root, fio);
		readRootNode.Write_AllTNodes (stream, fio);
	}

	// ignore cost when displaying nodes to users
	cost = old_cost;

	return stream;
}

template <class Whatever>
ostream & TNode<Whatever> ::
Write_AllTNodes (ostream & stream, fstream * fio) const 
{
	if (left) {
		TNode<Whatever> readLeftNode (left, fio);
		readLeftNode.Write_AllTNodes (stream, fio);
	}
	stream << *this;
	if (right) {
		TNode<Whatever> readRightNode (right, fio);
		readRightNode.Write_AllTNodes (stream, fio);
	}

	return stream;
}
