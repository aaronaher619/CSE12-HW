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
  File Name:      Driver.h                                                  \
                                                                            \
  Description:    This program has the UCSDStudent class which sets has     \ 
                  constructor and overloaded operator methods.              \
****************************************************************************/
#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;

//=========================================================================
// class UCSDStudent
//
// Description: Holds the info for a UCSD student including their name and
//              number. Can assign a number and get the name. Has a 
//              constuctor and overloaded operator methods.
//
// Data Fields:
//     name (char)  - the student's name
//     studentnum (long)  - the student number
//
// Public functions:
//     UCSDStudent - Constructor for the class to set values
//     UCSDStudent - Default Constuctor for the class, sets name
//     operator const char * - Returns the student's name
//     long operator > - Checks if one Student is greater than another
//     long operator == - Checks if the two Student objects are equal
//========================================================================
class UCSDStudent {
  friend ostream & operator <<(ostream &, const UCSDStudent &);
  char name[16];
  long studentnum;
 public:
  
  /**
   *Constuctor that sets the values of student
   *
   *@param name of student
   *@param Student's number to be set
   */
  UCSDStudent(char * nm, long val = 0) : studentnum (val) { 
    memset (name, '\0', sizeof (name)); 
    strcpy (name, nm);  
  }
  
  /**
   * Default Constuctor that sets the values of student name
   */
  UCSDStudent (void) : studentnum (0) {  
    memset (name, '\0', sizeof (name));  
  } 

  /**
   * This gets the name of the student
   *
   * @return  the student's name
   */
  operator const char *(void) const {
    return name;
  }

  
  /**
   * This checks if the two Student objects are equal
   *
   * @return  true of false if equal or not
   */
  long operator ==(const UCSDStudent & vvv) const {
    return !strcmp(name, vvv.name);
  }

  
  /**
   * This checks which of the two Student objects is bigger
   *
   * @return  true if this object is greater that the other object
   */
  long operator >(const UCSDStudent & vvv) const {
    return (strcmp(name, vvv.name) > 0) ? 1 : 0;
  }
};

#endif
