#ifndef STACK_H
#define STACK_H

/* DO NOT CHANGE:  This file is used in evaluation 
 * Changing function signatures will result in a 25% deduction.
 * */

#include <iostream>
#include "List.h"
using namespace std;

class Stack : private List {
public:
	List :: IsEmpty;
	List :: NumElements;
	List :: Write;

	Base * Pop (void);
	long Push (Base *);
	Base * Top (void) const;
};

#endif
