/****************************************************************************

                                                Aaron Hernandez
                                                CSE 12, Spring 2017
                                                May 10, 2017
                                                cs12xcb
                              Assignment 6

File Name:      Driver.c
Description:    This program has the UCSDStident class which sets has 
                constructor, deconstuctor, setters and getter methods.
                Then it drives the table by checking input and calls
                methods accordingly.

****************************************************************************/
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include "Base.h"
#include "SymTab.h"
using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

//=========================================================================                                                                         
// class UCSDStudent                                                                                                                                  
//                                                                                                                                                 
// Description: Holds the info for a UCSD student including their name and                                                                         
//              number. Can assign a number and get the name. Also sets the                                                                         
//              hashcode and can check if another class object is equal to                                                                            
//              it. Lastly it has a write and deconstuctor method.                                                                   
//                                                                                                                                                           
// Data Fields:                                                                                                                                              
//     name (char *)  - the student's name                                                                                       
//     studentnum (long)  - the student number                                                                                              
//                                                                                                                                                           
// Public functions:                                                                                                                                         
//     UCSDStudent - Constructor for the class to set values                                                                                               
//     ~UCSDStudent - Deconstuctor for the class 
//     operator char * - returns the student's name                                                                           
//     operator long - return the hash number                                                                  
//     long operator == - checks if the two objects are equal                                                                                   
//     ostream & Write - prints the objects details                                                                                          
//==========================================================================
class UCSDStudent : public Base {
	char * name;
	long studentnum;
public:
	/**                                                                                                                                     
         *Constuctor that sets the values of student                                                                                        
         *                                                                                                                                  
         *@param name of student                                                                                             
         *@param Student's number to be set                                                                                            
	 */
	UCSDStudent (char * nm, long sn) :
	name (strdup (nm)), studentnum (sn) {}
	
	/**                                                                                                                                                  
         *Deconstuctor that frees the memory
         */
	~UCSDStudent (void) {
	  free (name);
	}
	
	/**               
         * This gets the name of the student                                                                                        
         *                                                                                                                                                   
         * @return  the student's name                                                                                                                 
         */
	operator char * (void) {  
	  return name;  
	}
  
	//hash function
	operator long (void);  
  
	/**                                                                                                                                                  
         * This checks if the two base objects are equal                                                                                          
         *                                                                                                                                                   
         * @return  true of false if equal                                                                                                             
         */
	long operator == (Base & base) {  
	  return *this == base;  
	}  
		
	//Write function
	ostream & Write (ostream & stream) {
	  return stream << "name: " << name
			<< " Studentnum: " << studentnum;
	}
	
};
/**                                                                                                                                                   
 * This method determines the ASCII sum of the student name                                                                                           
 *                                                                                                                                                    
 * @return  the ASCII sum                                                                                                                             
 */

UCSDStudent :: operator long (void){
  long sum = 0;

  //loop that sums up the ASCII sum
  for (long index = 0; name[index]; index++){
    sum+= name[index];
  }

  //return the sum
  return sum;
}




int main (int argc, char * const * argv) {
	SymTab ST (5);
	char buffer[128];
	char command;
	long number;
	char option;

	/* initialize debug variable */
	HashTable::Set_Debug(0);

	 /* check command line options for debug display */
	 while ((option = getopt (argc, argv, "x")) != EOF) {

		 switch (option) {
			 case 'x': HashTable::Set_Debug(1);
				 break;
		 	}
	} 

	ST.Write (cout << "Initial Symbol Table:\n");
	while (cin) {
		command = NULL;	// reset command each time in loop
		cout << "Please enter a command:  ((i)nsert, (l)ookup, (w)rite):  ";
		cin >> command;

		switch (command) {
		case 'i':
			cout << "Please enter UCSD student name to insert:  ";
			cin >> buffer;	// formatted input

			cout << "Please enter UCSD student number:  ";
			cin >> number;

			// create student and place in symbol table
			if(!ST.Insert (new UCSDStudent (buffer, number))){

				ST.Write (cout << "\nFinal Symbol Table:\n");
				exit (0);
			}
			break;

		case 'l': {
			Base * found;	// whether found or not

			cout << "Please enter UCSD student name to lookup:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			found = ST.Lookup (& stu);
			
			if (found)
				found->Write (cout << "Student found!!!\n") << "\n";
			else
				cout << "Student " << buffer << " not there!\n";
			}
			break;

		case 'w':
			ST.Write (cout << "The Symbol Table contains:\n");
		}
	}
	ST.Write (cout << "\nFinal Symbol Table:\n");
}

