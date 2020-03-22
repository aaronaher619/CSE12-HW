/****************************************************************************   
                                                                                
                                                Aaron Hernandez                 
                                                CSE 12, Spring 2017             
                                                May 17, 2017                    
                                                cs12xcb                         
                              Assignment 7                                      
                                                                                
File Name:      Driver.c                                                        
Description:    This program has the UCSDStident class which sets has           
                constructor, deconstuctor, setters and getter methods.          
                Then it drives the tree by checking input and calls            
                methods accordingly.                                            
                                                                                
****************************************************************************/
#include <cstdlib>
#include <iostream>
#include <string.h>
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
//========================================================================
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
  UCSDStudent (char * nm, long val=0) :  
  name (strdup (nm)), studentnum (val) {}

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
  operator const char * (void) const{  
    return name;  
  }  
  
  /**                                                                                                                                                  
   * This checks if the two base objects are equal                                                                                                      
   *                                                                                                                                                   
   * @return  true of false if equal                                                                                                                    
   */
  long operator == (const Base & bbb) const {  
    return ! strcmp (name, bbb);  
  }  
  
  /**                                                                                                                                                  
   * This checks which o fthe two objects is bigger                                                                                                      
   *                                                                                                                                                   
   * @return  the object that is bigger                                                                                                                    
   */
  long operator > (const Base & bbb) const {  
    return (strcmp (name, bbb) > 0) ? 1 : 0;  
  }  
  
  //Write function 
  ostream & Write (ostream & stream) const {
    return stream << "name:  " << name 
		  << "  studentnum:  " << studentnum;
  }
};

int main (int argc, char * const * argv) {
	char buffer[80];
	char command;
	long number;

	Tree::Set_Debug(0);

	if (argc != 1 && strcmp("-x", argv[1]) == 0) {
		Tree::Set_Debug(1);
	}
	
	SymTab * ST;
	ST = new SymTab ("UCSDStudentTree");
	ST->Write (cout << "Initial Symbol Table:\n");

	while (cin) {
		command = NULL;		// reset command each time in loop
		cout <<  "Please enter a command:  ((a)llocate, (i)nsert, " <<
			"(l)ookup, (r)emove, (w)rite):  ";
		cin >> command;

		switch (command) {

		case 'a':
			cout << "Please enter name of new Tree to allocate:  ";
			cin >> buffer; 

			delete ST;
			ST = new SymTab (buffer);

			break;

		case 'i':
			cout << "Please enter UCSD student name to insert:  ";
			cin >> buffer;	// formatted input

			cout << "Please enter UCSD student number:  ";
			cin >> number;

			// create student and place in symbol table
			ST->Insert (new UCSDStudent (buffer, number));
			break;

		case 'l': {
			const Base * found;	// whether found or not

			cout << "Please enter UCSD student name to lookup:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			found = ST->Lookup (&stu);
			
			if (found)
				found->Write (cout << "Student found!\n") << "\n";
			else
				cout << "student " << buffer << " not there!\n";
			}
			break;
		
		case 'r': {
			Base * removed;	// data to be removed

			cout << "Please enter UCSD student name to remove:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			removed = ST->Remove (&stu);

			if (removed)
				removed->Write (cout << "Student removed!\n") << "\n";
			else
				cout << "student " << buffer << " not there!\n";
			}
			break;


		case 'w':
			ST->Write (cout << "The Symbol Table contains:\n");
		}
	}

	ST->Write (cout << "\nFinal Symbol Table:\n");
	delete ST;
}
