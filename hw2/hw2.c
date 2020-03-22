/****************************************************************************

                                                        Aaron Hernandez
                                                        CSE 12, Spring 2017
                                                        April 10, 2017
                                                        cs12xcb
                                Assignment Two

File Name:      hw2.c
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.  At termination, program
                outputs sizes of various types of C/C++ pre defined types.

****************************************************************************/

/* declare fputc/fgetc */
#include <stdio.h>
#include <getopt.h>

/* define some program-wide constants */
#define ASCII_ZERO '0'
#define COUNT ((long) (sizeof (long) << 1))
#define ERROR -1
#define FALSE 0
#define MAXLENGTH 80
#define OFFSET ('a' - 'A')
#define SPACE ' '
#define TRUE 1

#define DECIMAL 10
#define HEX 16

/* define the keyword "NULL" as 0 */
#ifdef NULL
#undef NULL
#endif
#define NULL 0

/* declarations for functions defined in this file */
void baseout (long number, long base, FILE *stream);
void clrbuf (int);
long decin (void);
void decout (unsigned long, FILE *);
void digiterror (int, long *, const char *);
long getaline (char *, int);
void hexout (unsigned long, FILE *);
void newline (FILE *);
long writeline (const char *, FILE *);

/* array for input checking and for output */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* messages */
const char CAUSED_OVERFLOW[] =  " caused overflow!!!\n";
const char DIGIT_STRING[] = "digit ";
const char REENTER_NUMBER[] = "\nPlease reenter number: ";
const char OUT_OF_RANGE[] = " out of range!!!\n";

/* debug messages */
const char DEBUG_GETALINE[] =
        "[*DEBUG:  The length of the string just entered is ";
const char DEBUG_WRITELINE[] =
        "\n[*DEBUG:  The length of the string displayed is ";
static long debug_on = FALSE;

/*--------------------------------------------------------------------------
    Copy your function header and code for baseout() from hw1
--------------------------------------------------------------------------*/
void baseout (long number, long base, FILE * stream) {
  int hexCounter = 0;
  int i = 0;
  char num[16];
  long newNum = number;
  long modulo;
  char c;

  //If base is in decimal then run decout instead
  if (base == 10){
    decout(number, stream);
    return;
  }

  //If base is 16 then print number in hexadecimal
  if ((base == 16) || (base != 10)){
      
      //runs loop for reading the number and turning it into hex base and stooing the
      //hex number into a chare array
      for(; hexCounter < 16; ++hexCounter){
	modulo = newNum % base;
	if (modulo == 0) { num[15-hexCounter] = '0';}
	else if (modulo == 1) { num[15-hexCounter] = '1';}
	else if (modulo == 2) { num[15-hexCounter] = '2';}
	else if (modulo == 3) { num[15-hexCounter] = '3';}
	else if (modulo == 4) { num[15-hexCounter] = '4';}
	else if (modulo == 5) { num[15-hexCounter] = '5';}
	else if (modulo == 6) { num[15-hexCounter] = '6';}
	else if (modulo == 7) { num[15-hexCounter] = '7';}
	else if (modulo == 8) { num[15-hexCounter] = '8';}
	else if (modulo == 9) { num[15-hexCounter] = '9';}
	else if (modulo == 10) { num[15-hexCounter] = 'A';}
	else if (modulo == 11) { num[15-hexCounter] = 'B';}
	else if (modulo == 12) { num[15-hexCounter] = 'C';}
	else if (modulo == 13) { num[15-hexCounter] = 'D';}
	else if (modulo == 14) { num[15-hexCounter] = 'E';}
	else if (modulo == 15) { num[15-hexCounter] = 'F';}
	newNum = newNum / base;
      }
    }

    //sends the char array to get printed
    if(base != 10){
      for (; i < 16; ++i){
	c = num[i];
	fputc(c, stream);
      }
    }
    return;
}

/*-------------------------------------------------------------------------- 
Function Name:          clrbuf 
Purpose:                When called, this function will clear stdin. 
Description:            This function checks to see if the incoming 
                        parameter is already '\n' indicating that stdin 
                        is already clear.  If stdin is already clear, this 
                        function does nothing.  Otherwise, this function 
                        calls "fgetc" in a loop until stdin is clear. 
Input:                  character:  the most recent character received from a 
                                previous call to fgetc. 
Result:                 stdin cleared.  Nothing returned. 
--------------------------------------------------------------------------*/  
void clrbuf (int character) {  
	
  //reinitializes character  to the most recent character from stdin until \n is reached
  while(1){
    if (character == '\n'){ break;}
    else {character = (char) fgetc(stdin);}
  }
}


/*--------------------------------------------------------------------------
Function Name:          decin
Purpose:                This function accepts integer input from from the user.
Description:            This function processes user input in a loop that ends
                        when the user enters either a valid number or EOF.
                        If EOF is entered, EOF is returned.  Otherwise each
                        character entered is checked to verify that it is
                        numeric.  Non-numeric input is identified, the user
                        is notified, reprompted, and the loop begins again.
                        Once the input is verified to be valid, a series
                        of multiplication by 10 and addition can take
                        place to convert the ASCII characters entered into
                        a numeric quantity.
Input:                  None.
Result:                 The number entered or EOF.
--------------------------------------------------------------------------*/
long decin (void) {
  //The main loop that starts the reading the first char from System.in
  //restarts everytime there is an error
  while(1){
    int temp = fgetc(stdin);
    char c1  = (char) temp;
    long sum;
    long oldSum;
    char c;
    int nextNum;

    //checks if nothing was entered
    if (c1 == '\n') { return 0;}

    //checks for ^d
    if(c1 == EOF){
      return EOF;
    }

    //checks for out of range chars
    if (((int) c1 < 48) || ((int) c1 > 57)){
      digiterror((int) c1, &sum, OUT_OF_RANGE);
      continue;
    }

    sum = ((int) c1) - 48;

    //The inner loop that continues reading the rest of the chars from System.in
    while(1){
      c  =  (char) fgetc(stdin);
      nextNum = ((int) c) - 48;
      c1 = c;

      //cheacks for the end of the input steam and returns the final number
      if (c  == '\n') {
	return sum;
      }

      //checks for out of range int
      if ((nextNum < 0) || (nextNum > 9)){
	digiterror((nextNum + 48), &sum,  OUT_OF_RANGE);
	break;
      }

      oldSum = sum;
      sum = sum * 10;

      //checks for Overflow
      if (oldSum != (sum/10)){
	digiterror((nextNum) + 48, &sum, CAUSED_OVERFLOW);
	break;
      }
      sum = sum + nextNum;
    }
  }
}


/*--------------------------------------------------------------------------
Function Name:         List the function name. 
Purpose:               Say why the function exists in a sentence or two. 
Description:           Describe the underlying algorithm of how the function 
                       is implemented in a few sentences. 
Input:                 A list of the parameters with brief explanation of each. 
Result:                A list of possible outcomes when the function is called. 
                       Describe return value. 
--------------------------------------------------------------------------*/
void decout (unsigned long number, FILE * stream) {
  int counter = 0;
  int i = 0;
  char num[16];
  long newNum = number;
  long modulo;
  char c;
  int once = 1;

  //Checks if the number is zero and prints 0 then exits
  if (newNum == 0){
    c = '0';
    fputc(c, stream);
    once = 0;
    i = 16;
    counter = 16;
    return;
  }

  //reads the number and converts it to an array of chars
  for(; counter < 16; ++counter){
    modulo = newNum % 10;
    if (modulo == 0) { num[15-counter] = '0';}
    else if (modulo == 1) { num[15-counter] = '1';}
    else if (modulo == 2) { num[15-counter] = '2';}
    else if (modulo == 3) { num[15-counter] = '3';}
    else if (modulo == 4) { num[15-counter] = '4';}
    else if (modulo == 5) { num[15-counter] = '5';}
    else if (modulo == 6) { num[15-counter] = '6';}
    else if (modulo == 7) { num[15-counter] = '7';}
    else if (modulo == 8) { num[15-counter] = '8';}
    else if (modulo == 9) { num[15-counter] = '9';}
    newNum = newNum / 10;
  }

  //finds the index where the number represented by chars starts
  for(; once == 1; ++i){
    if(num[i] == '0') {
    }
    if((num[i] != '0') || (i == 15)) {
      once = 0;
      break;
    }
  }

  //sends the array of chars to print the number
  for (; i < 16; ++i){
    c = num[i];
    fputc(c, stream);
  }
}

/*--------------------------------------------------------------------------
Function Name:          digiterror
Purpose:                This function handles erroneous user input.
Description:            This function reinitializes sum, displays and
                        error message to the user, and asks for fresh
                        input.
Input:                  character:  The character that began the problem.
                        sum:  A pointer to the sum to reinitialize.
                        message:  The message to display to the user.
Result:                 The message is displayed to the user.  sum is 0.
--------------------------------------------------------------------------*/
void digiterror (int character, long * sum, const char * message) {

	/* handle error */
	clrbuf (character);

	/* reset sum */
	*sum = 0;

	/* output error message */
	writeline (DIGIT_STRING, stderr);
	fputc (character, stderr);
	writeline (message, stderr);

	writeline (REENTER_NUMBER, stdout);
}


/*--------------------------------------------------------------------------
Function Name:          getaline
Purpose:                This function will read a string from the user.
Description:            This function gets input from the user via
                        calls to fgetc up to some maximum number of
                        characters.  Input is terminated when either the
                        maximum number of characters are entered, or
                        a newline character is detected.  If the user
                        enters more characters than the maximum, clrbuf
                        is called to remove extra characters which are
                        ignored.  Since this is routine accepts input,
                        if EOF is detected EOF is passed back to the main
                        program.
Input:                  message:  the destination array where input is stored.
                        maxlength:  the maximum number of non-NULL characters
                                allowed in the string + 1 for the NULL char.
Result:                 User input is stored in message.
                        EOF is returned when the user enters ^D.
                        Otherwise, the length of the message is returned.
--------------------------------------------------------------------------*/
long getaline (char * message, int maxlength) {
  int i;
  int temp = fgetc(stdin);
  char c = (char) temp;
  int length;

  //Checks for ^d
  if(c == EOF){
    return EOF;
  }

  //Adds the input string to the massage array until \n is detected
  for(i = 0; c != '\n'; ++i){
    //checks if message array is at maxlength
    if (i == (maxlength-1)) {
      clrbuf((char) fgetc(stdin));
      break;
    }

    message[i] = c;
    c = (char) fgetc(stdin);
  }
  maxlength = i;
  //Adds null at the end of the message array
  message[i] = '\0';

  //Adds the debug messages when debug_on is true
  if (debug_on){
    fprintf(stderr, DEBUG_GETALINE);
    decout(i, stderr);
    fputc(']', stderr);
  }
  length = i;
  ++i;

  //resets the array to only spaces
  for (; i < 80; ++i){
    message[i] = ' ';
  }

  return length;
}


/*--------------------------------------------------------------------------
Function Name:         hexout 
Purpose:               Prints a number in base 16 to the parameter FILE stream 
Description:           Goal is achieved via delegating to the baseout function 
Input:                 number:  the number to display 
                       stream:  where to display, likely stdout or stderr 
Result:                Number in base 16 is displayed. 
                       No return value. 
--------------------------------------------------------------------------*/
void hexout (unsigned long number, FILE * stream) {

  /* Output "0x" for hexidecimal. */
  writeline ("0x", stream);
  baseout (number, HEX, stream);
}

/*--------------------------------------------------------------------------
Function Name:         List the function name. 
Purpose:               Say why the function exists in a sentence or two. 
Description:           Describe the underlying algorithm of how the function 
                       is implemented in a few sentences. 
Input:                 A list of the parameters with brief explanation of each. 
Result:                A list of possible outcomes when the function is called. 
                       Describe return value. 
--------------------------------------------------------------------------*/
 void newline (FILE * stream) {
   fputc('\n', stream);
 }

/*--------------------------------------------------------------------------
Function Name:         List the function name. 
Purpose:               Say why the function exists in a sentence or two. 
Description:           Describe the underlying algorithm of how the function 
                       is implemented in a few sentences. 
Input:                 A list of the parameters with brief explanation of each. 
Result:                A list of possible outcomes when the function is called. 
                       Describe return value. 
--------------------------------------------------------------------------*/
long writeline (const char * message, FILE * stream) {
  int i = 0;

  //prints the message char by char
  while (message[i] != '\0'){
    fputc(message[i], stream);
    i++;
  }
  
  //Prints the debug message
  if(debug_on){
    fprintf(stderr, DEBUG_WRITELINE);
    decout(i+1, stderr);
    fputc(']', stderr);
    newline(stderr);
  }
  return i;
}

/*--------------------------------------------------------------------------
Function Name:          main
Description:            This function asks for input and displays output
Purpose:                This program asks the user to enter a string
                        and a number.  Computations are performed on the 
                        strings and numbers, and the results are displayed.
Description:            This function asks for input and displays output
                        in an infinite loop until EOF is detected.  Once EOF
                        is detected, the lengths of the types are displayed.
Input:                  None.
--------------------------------------------------------------------------*/
int main (int argc, char *const* argv) {
	char buffer[MAXLENGTH];      /* to hold string */
	long number;                  /* to hold number entered */
	long strlen;                  /* length of string */
	long base;		      /* to hold base entered */

	long array[10];               /* to show user where memory is allocated */
	long * ap = array;		/* to show user about addresses in memory */
	long ** app = &ap;		/* to show user about addresses in memory */
	long * apx = &array[0];	/* to show user about addresses in memory */
	char option;                 /* the command line option */

	/* initialize debug states */
	debug_on = FALSE;

	/* check command line options for debug display */
	while ((option = getopt (argc, argv, "x")) != EOF) {
		switch (option) {
			case 'x': debug_on = TRUE; break;
		}
	}

	/* infinite loop until user enters ^D */
	while (1) {
		writeline ("\nPlease enter a string:  ", stdout);
		strlen = getaline (buffer, MAXLENGTH);
		newline (stdout);

		/* check for end of input */
		if (strlen == EOF)
			break;

		writeline ("The string is:  ", stdout);
		writeline (buffer, stdout);

		writeline ("\nIts length is ", stdout);
		decout (strlen, stdout);
		newline (stdout);

		writeline ("\nPlease enter a decimal number:  ", stdout);
		if ((number = decin ()) == EOF)
			break;

		writeline ("\nPlease enter a decimal base:  ", stdout);
		if ((base = decin ()) == EOF)
			break;

		/* correct bases that are out of range */
		if (base < 2)
			base = 2;
		else if (base > 36)
			base = 36;

		newline (stdout);

		writeline ("Number entered in base ", stdout);
		decout (base, stdout);
		writeline (" is: ", stdout);
		baseout (number, base, stdout);

		writeline ("\nAnd in decimal is:  ", stdout);
		decout (number, stdout);

		writeline ("\nAnd in hexidecimal is:  ", stdout);
		hexout (number, stdout);

		writeline ("\nNumber entered multiplied by 8 is:  ", stdout);
		decout (number << 3, stdout);
		writeline ("\nAnd in hexidecimal is:  ", stdout);
		hexout (number << 3, stdout);

		newline (stdout);
	}

	writeline ("\nThe value of ap is:  ", stdout);
	decout ((long) ap, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) ap, stdout);
	newline (stdout);

	writeline ("The value of app is:  ", stdout);
	decout ((long) app, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) app, stdout);
	newline (stdout);

	writeline ("The value of apx is:  ", stdout);
	decout ((long) apx, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) apx, stdout);
	newline (stdout);

	writeline ("The value of ap + 1 is:  ", stdout);
	decout ((long) (ap+1), stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) (ap+1), stdout);
	newline (stdout);

	writeline ("The address of array[0] is:  ", stdout);
	decout ((long) &array[0], stdout);
	newline (stdout);

	writeline ("The address of array[1] is:  ", stdout);
	decout ((long) &array[1], stdout);
	newline (stdout);

	writeline ("The size of a float is:  ", stdout);
	decout (sizeof (float), stdout);
	newline (stdout);

	writeline ("The size of a double is:  ", stdout);
	decout (sizeof (double), stdout);
	newline (stdout);

	writeline ("The size of a long double is:  ", stdout);
	decout (sizeof (long double), stdout);
	newline (stdout);

	writeline ("The size of a char is:  ", stdout);
	decout (sizeof (char), stdout);
	newline (stdout);

	writeline ("The size of an int is:  ", stdout);
	decout (sizeof (int), stdout);
	newline (stdout);

	writeline ("The size of a short is:  ", stdout);
	decout (sizeof (short), stdout);
	newline (stdout);

	writeline ("The size of a short int is:  ", stdout);
	decout (sizeof (short int), stdout);
	newline (stdout);

	writeline ("The size of a long is:  ", stdout);
	decout (sizeof (long), stdout);
	newline (stdout);

	writeline ("The size of a long int is:  ", stdout);
	decout (sizeof (long int), stdout);
	newline (stdout);

	writeline ("The size of a long long is:  ", stdout);
	decout (sizeof (long long), stdout);
	newline (stdout);

	writeline ("The size of a signed is:  ", stdout);
	decout (sizeof (signed), stdout);
	newline (stdout);

	writeline ("The size of a signed char is:  ", stdout);
	decout (sizeof (signed char), stdout);
	newline (stdout);

	writeline ("The size of a signed short is:  ", stdout);
	decout (sizeof (signed short), stdout);
	newline (stdout);

	writeline ("The size of a signed short int is:  ", stdout);
	decout (sizeof (signed short int), stdout);
	newline (stdout);

	writeline ("The size of a signed int is:  ", stdout);
	decout (sizeof (signed int), stdout);
	newline (stdout);

	writeline ("The size of a signed long is:  ", stdout);
	decout (sizeof (signed long), stdout);
	newline (stdout);

	writeline ("The size of a signed long int is:  ", stdout);
	decout (sizeof (signed long int), stdout);
	newline (stdout);

	writeline ("The size of a signed long long is:  ", stdout);
	decout (sizeof (signed long long), stdout);
	newline (stdout);

	writeline ("The size of an unsigned is:  ", stdout);
	decout (sizeof (unsigned), stdout);
	newline (stdout);

	writeline ("The size of an unsigned char is:  ", stdout);
	decout (sizeof (unsigned char), stdout);
	newline (stdout);

	writeline ("The size of an unsigned short is:  ", stdout);
	decout (sizeof (unsigned short), stdout);
	newline (stdout);

	writeline ("The size of an unsigned short int is:  ", stdout);
	decout (sizeof (unsigned short int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned int is:  ", stdout);
	decout (sizeof (unsigned int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long is:  ", stdout);
	decout (sizeof (unsigned long), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long int is:  ", stdout);
	decout (sizeof (unsigned long int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long long is:  ", stdout);
	decout (sizeof (unsigned long long), stdout);
	newline (stdout);

	writeline ("The size of a void pointer is:  ", stdout);
	decout (sizeof (void *), stdout);
	newline (stdout);

	writeline ("The size of a character pointer is:  ", stdout);
	decout (sizeof (char *), stdout);
	newline (stdout);

	writeline ("The size of an int pointer is:  ", stdout);
	decout (sizeof (int *), stdout);
	newline (stdout);

	writeline ("The size of a long pointer is:  ", stdout);
	decout (sizeof (long *), stdout);
	newline (stdout);

	writeline ("The size of a float pointer is:  ", stdout);
	decout (sizeof (float *), stdout);
	newline (stdout);

	writeline ("The size of a double pointer is:  ", stdout);
	decout (sizeof (double *), stdout);
	newline (stdout);

	writeline ("The size of a long double pointer is:  ", stdout);
	decout (sizeof (long double *), stdout);
	newline (stdout);

	newline (stdout);

	return 0;
}
