/***************************************************************************\
                                                                            \ 
                                                 Aaron Hernandez            \
                                                 Othman Akel                \
                                                 CSE 12, Spring 2017        \ 
                                                 June 9, 2017               \ 
                                                 cs12xcb                    \
                                                 cs12xaa                    \
                               Assignment 9                                 \ 
                                                                            \
  File Name:      Driver.c                                                  \
                                                                            \
  Description:    This program is the main driver for the Binary tree where \
                  it reads input from a user or file and does the appropiate\
                  steps to accomplish them.                                 \
****************************************************************************/
#include <iostream>
#include <cstdio>
#include <string>
#include <getopt.h>
#include "Driver.h"
#include "SymTab.h"
#include <fstream> 

using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

ostream & operator << (ostream & stream, const UCSDStudent & stu) {
        return stream << "name:  " << stu.name
                << " with studentnum:  " << stu.studentnum;
}

int main (int argc, char * const * argv) {
        char buffer[BUFSIZ];
        char command;
        long number;
        char option;
	istream * is = &cin;
	ostream * os = &cout;
        
        SymTab<UCSDStudent>::Set_Debug_Off ();

        while ((option = getopt (argc, argv, "x")) != EOF) {

        switch (option) {
                case 'x': SymTab<UCSDStudent>::Set_Debug_On ();
                        break;
                }       
        }
        
        SymTab<UCSDStudent> ST("Driver.datafile");
        ST.Write (cout << "Initial Symbol Table:\n" );

        while (cin) {
                command = NULL;         // reset command each time in loop
                *os << "Please enter a command ((f)ile, (i)nsert, "
                        << "(l)ookup, (r)emove, (w)rite):  ";
                *is >> command;
		

		//Checks for EOF
		if (*is == NULL) {
		  
		  //Resets the is pointer
		  if (*is != cin) {
		    delete is;
		    is = &cin;		    
		  }

		  //Resets the os pointer
		  if (*os != cout) {
		    delete os;
		    os = &cout;
		  }		  
		}
                
		switch (command) {
		  
		  //Case if f was inputed by the user
		case 'f': {
		  //prompts for the user
		  *os << "Enter file name for commands: ";
		  *is >> buffer;
		  
		  //Checks for end of file
		  if (*is != cin){
		    delete is;
		  }

		  //Resets the is pointer
		  is = new ifstream(buffer);

		  //Checks for end of file
		  if (*os != cout){
		    delete os;
		  }
		  //Resets the os pointer
		  os = new ofstream("/dev/null");
		  break;
			
		}
		        

                case 'i': {
                        *os << "Please enter UCSD student name to insert:  ";
                        *is >> buffer;  // formatted input

                        *os << "Please enter UCSD student number:  ";
                        *is >> number;

                        UCSDStudent stu (buffer, number);

                        // create student and place in symbol table
                        ST.Insert (stu);
                        break;
                }
                case 'l': { 
                        unsigned long found;    // whether found or not

                        *os << "Please enter UCSD student name to lookup:  ";
                        *is >> buffer;  // formatted input

                        UCSDStudent stu (buffer, 0);
                        found = ST.Lookup (stu);
                        
                        if (found)
                                cout << "Student found!!!\n" << stu << "\n";
                        else
                                cout << "student " << buffer << " not there!\n";
                        break;
                        }
                case 'r': { 
                        unsigned long removed;

                        *os << "Please enter UCSD student name to remove:  ";
                        *is >> buffer;  // formatted input

                        UCSDStudent stu (buffer, 0);
                        removed = ST.Remove(stu);

                        if (removed)
                                cout << "Student removed!!!\n" << stu << "\n";
                        else
                                cout << "student " << buffer << " not there!\n";
                        break;
                }
                case 'w':
                        ST.Write (cout << "The Symbol Table contains:\n");
                }
        }

        ST.Write (cout << "\nFinal Symbol Table:\n");

	if (ST.GetOperation() != 0) {
	  cout << "\nCost of operations: ";
	  cout << ST.GetCost();
	  cout << " tree accesses";
	  
	  cout << "\nNumber of operations: ";
	  cout << ST.GetOperation();
	  
	  cout << "\nAverage cost: ";
	  cout << (((float)(ST.GetCost()))/(ST.GetOperation()));
	  cout << " tree accesses/operation\n";
	}
	
	else
	  cout << "\nNo cost information available.\n";
}
