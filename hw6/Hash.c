/****************************************************************************

                                                Aaron Hernandez
                                                CSE 12, Spring 2017
                                                May 10, 2017
                                                cs12xcb
                              Assignment 6

File Name:      Hash.c
Description:    This program creates a new table. Then you can insert or 
                lookup elements from the table which calls locate. Then
                it free the dynamic memory on exit. It can also check for the
                debug flag and finally has a write function.

****************************************************************************/
#include <cstdlib>
#include <string>
#include "Hash.h"

using namespace std;

static const char AND[] = " and ";
static const char DEBUG_ALLOCATE[] = " - Allocated]\n";
static const char DEBUG_DEALLOCATE[] = " has been deallocated]\n";
static const char DEBUG_LOCATE[] = " - Locate]\n";
static const char DEBUG_LOOKUP[] = " - Lookup]\n";
static const char BUMP[] = "[Bumping To Next Location...]\n";
static const char COMPARE[] = " - Comparing ";
static const char FOUND_SPOT[] = " - Found Empty Spot]\n";
static const char HASH[] = "[Hash Table ";
static const char HASH_VAL[] = "[Hash value is: ";
static const char INSERT[] = " - Inserting ";
static const char PROCESSING[] = "[Processing ";
static const char TRYING[] = "[Trying index ";

bool HashTable :: debug = 0;
int HashTable :: counter = 0;

void HashTable :: Set_Debug (int option)

/***************************************************************************
% Routine Name : HashTable :: Set_Debug (public)
% File :         hash.c
% 
% Description :  This function sets debug mode on or off
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% option             true should set debug mode on, false should set debug
%                    mode off.
***************************************************************************/

{
  debug = option;
}


HashTable :: HashTable (int sz) : size (sz),
	table_count(++counter), occupancy (0), table (new Base *[sz]), 
	probeCount (new int[sz])
/***************************************************************************
% Routine Name : HashTable :: HashTable (public)
% File :         hash.c
% 
% Description :  This function allocates an initializes the memory
%                associated with a hash table.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% size               The number of elements for the table...MUST BE PRIME!!!
***************************************************************************/

{
  /*Prints Debug message*/
  if(debug){
    cerr<<HASH<<counter<<DEBUG_ALLOCATE;
  }

  /* initialize all table elements*/ 
   for(long i = 0; i < size; i++){
     table[i] = NULL;
     probeCount[i] = NULL;
     }
}	/* end: HashTable */


HashTable :: ~HashTable (void)
/***************************************************************************
% Routine Name : HashTable :: ~HashTable  (public)
% File :         hash.c
% 
% Description :  deallocates memory associated with the Hash Table.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
  /* call function to delete individual elements */
  for(long i = 0; i < size; i++){
    if(table[i] == NULL ){continue;}
    char * eName = *table[i];
    free (eName);
    delete table[i];
  }

  /* delete table itself */
  delete probeCount;
  delete table;
}	/* end: ~HashTable */


bool HashTable :: Insert (Base * element, int initialCount)
/***************************************************************************
% Routine Name : HashTable :: Insert (public)
% File :         hash.c
% 
% Description : This function will insert the element in the hash table.
%		If the element cannot be inserted, false will be returned.
%		If the element can be inserted, the element is inserted
%		and true is returned.  Duplicate insertions will
%		cause the existing element to be deleted, and the duplicate
%		element to take its place.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert.
% recursiveCall      Whether or not we are entering from recursion
% <return>           true or false indicating success or failure of insertion
***************************************************************************/

{ 
  //Contains the element name
  char * eName = *element;

  /*Declares the bumped values*/
  Base * bumped;
  int bumpedCount;

  /*Checks if element is null*/
  if (element == NULL){
    return false;
  }

  //Checks for Debug
  if(debug){
    cerr << HASH << counter << INSERT << eName << "]\n";
    cerr << HASH << counter << DEBUG_LOCATE;
  }

  /*Sets count and calls locate to set index*/
  count = initialCount;
  if (!(Locate(element))){
    return false;
  }

  /*Uses index to initialize the bumped values*/
  bumped = table[index];
  bumpedCount = probeCount[index];

  /*inserts the element in the table at the index*/
  table[index] = element;
  probeCount[index] = count;

  /*If an element needs to be bumped then it recursively calls it*/
  if((bumpedCount < count) && (bumpedCount > 0)){

    //Checks for Debug
    if(debug){cerr << BUMP;}

    Insert(bumped, bumpedCount);
  }

  /*Returns success*/
  return true;
}


bool HashTable :: Locate (Base * element)
/**********************************************************************
% Routine Name : HashTable :: Locate (private)
% File :         HashTable.c
% 
% Description : This function will locate the location in the
%               table for the insert or lookup.
%
% Parameters descriptions :
%  
% name               description
% ------------------ -------------------------------------------------
% element            The element needing a location.
% <return>           true if tem found, or false if not found
**********************************************************************/
{
  int newIndex;
  long sum = *element;
  int initialLoc = sum % size;
  int inc = sum % (size - 1) + 1;
  newIndex = initialLoc;
  char * eName = * element;
  char * eName2;

  //Checks for Debug
  if(debug){
    cerr << PROCESSING << eName << "]\n";
    cerr << HASH_VAL << sum << "]\n";
  }

  //Main loop that sets index
  while(true){

    //Checks for Debug
    if(debug){
      cerr << TRYING << newIndex << "]\n";
    }

    //checks if the index is empty
    if(table[newIndex] == NULL){

      //Checks for Debug
      if(debug){
	cerr << HASH << counter << FOUND_SPOT;
      }

      index = newIndex;
      ++occupancy;
      return true;
    }
    
    eName2 = *(table[newIndex]);

    //Checks for Debug
    if(debug){
      cerr << HASH << counter << COMPARE << eName << AND << eName2 << "]\n";
    }

    //Checks if the element is equal to the element at the index
    if(element == table[newIndex]){
      index = newIndex;
      return true;
    }

    //Checks if the element has a lower probe count than the element at the index
    else if(probeCount[newIndex] < count){
      index = newIndex;
      return true;
    }

    //Checks if the index has exhausted the table
    else if(count == size){
      index = initialLoc;
      return false;
    }

    //updates the count and reinitializes index
    ++count;
    newIndex = (newIndex + inc) % size;
  }
}

Base * HashTable :: Lookup (Base * element)
/***************************************************************************
% Routine Name : HashTable :: Lookup (public)
% File :         hash.c
% 
% Description : This function will lookup the element in the hash table.  If
%               found a pointer to the element is returned.  If the element
%               is not found, NULL will be returned to the user.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert or to lookup.
% <return>           A pointer to the element if found, else NULL
***************************************************************************/

{
  //Checks for Debug
  if(debug){
    cerr << HASH << counter << DEBUG_LOOKUP;
  }

  //Sets the count to 1 and uses locate to find if element is in the table
  count = 1;
  if(Locate(element)){

    //Checks for debug
    if(debug){cerr << HASH << counter << DEBUG_LOCATE;}

    //Checks if index of table is empty
    if(table[index] == NULL){
      return NULL;
    }

    //Checks if item hasn't traveled as long
    if(probeCount[index] < count){
      return NULL;
    }

    //return the found element
    return table[index];
  }
  return NULL;
}
   
ostream & HashTable :: Write (ostream & stream)
/***************************************************************************
% Routine Name : HashTable :: Write (public)
% File :         hash.c
% 
% Description : This funtion will output the contents of the hash table
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
	stream << "Hash Table " << table_count << ":\n"
	       << "size is " << size << " elements, "
	       << "occupancy is " << occupancy << " elements.\n";

	/* go through all table elements */
	for (int index = 0; index < size; index++)
		if (table[index])
			table[index]->Write(stream << "at index "
			<< index << ": ") << " with probeCount: "
			<< probeCount[index] << "\n";
	return stream;
}	/* end: Write */
