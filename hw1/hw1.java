/**
 * The hw1 class is a direct port of hw1.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.
 */

import java.io.*;      // System.out

public class hw1 {
	private static final long COUNT = 16;	// number of hex digits to display
	private static final long DECIMAL = 10;	// to indicate base 10
	private static final long HEX = 16;		// to indicate base 16

	private static final char digits[] = 	// used for ASCII conversion
		new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();
	private static long hexCounter = 0; // counter for the number hex digits


	/**
	* Takes in a positive number and displays in a given base.
        *
        * @param    Numeric value to be displayed.
        * @param    Base to used to display number.
        * @param    Where to display, likely System.out or System.err.
        */
	private static void baseout (long number, long base, PrintStream stream) {
	    char[] num = new char[16]; 
	    long newNum = number;
	    if (true){
		long modulo;
		for(int hexCounter =0; hexCounter < 16; ++hexCounter){
		    modulo = newNum % base;
		    if (modulo == 0) { num[num.length-1-hexCounter] = '0';}
		    else if (modulo == 1) { num[num.length-1-hexCounter] = '1';}
		    else if (modulo == 2) { num[num.length-1-hexCounter] = '2';}
		    else if (modulo == 3) { num[num.length-1-hexCounter] = '3';}
		    else if (modulo == 4) { num[num.length-1-hexCounter] = '4';}
		    else if (modulo == 5) { num[num.length-1-hexCounter] = '5';}
		    else if (modulo == 6) { num[num.length-1-hexCounter] = '6';}
		    else if (modulo == 7) { num[num.length-1-hexCounter] = '7';}
		    else if (modulo == 8) { num[num.length-1-hexCounter] = '8';}
		    else if (modulo == 9) { num[num.length-1-hexCounter] = '9';}
		    else if (modulo == 10) { num[num.length-1-hexCounter] = 'A';}
		    else if (modulo == 11) { num[num.length-1-hexCounter] = 'B';}
		    else if (modulo == 12) { num[num.length-1-hexCounter] = 'C';}
		    else if (modulo == 13) { num[num.length-1-hexCounter] = 'D';}
		    else if (modulo == 14) { num[num.length-1-hexCounter] = 'E';}
		    else if (modulo == 15) { num[num.length-1-hexCounter] = 'F';}
		    newNum = newNum / base;			
		}
	    }
	    for (int i = 0; i < 16; ++i){
		stream.print(num[i]);
	    }
	}


        /**
        * Takes in a positive number and displays it in decimal.
        *
        * @param    positive numeric value to be displayed.
        * @param    Where to display, likely System.out or System.err.
        */
	public static void decout (long number, PrintStream stream) {
	    stream.print(number);
	}


        /**
        * Displays a single character.
        *
        * @param    Character to display.
        * @param    Where to display, likely System.out or System.err.
        */
        public static void fputc(char CharToDisp, PrintStream stream) {

                // Print a single character.
                stream.print (CharToDisp);   

                // Flush the system.out buffer, now. 
                stream.flush ();
        }


        /**
        * Takes in a positive number and displays it in hex.
        *
        * @param    A positive numeric value to be displayed in hex.
        * @param    Where to display, likely System.out or System.err.
        */
        public static void hexout (long number, PrintStream stream) {

                // Output "0x" for hexidecimal.
                writeline ("0x", stream);
                baseout (number, HEX, stream);
        }


        /**
        * Prints out a newline character.
        * @param    Where to display, likely System.out or System.err.
        *
        */
        public static void newline ( PrintStream stream ) {
	    stream.print("\n");
        }


        /**
        * Prints out a string.
        *
        * @param    A string to print out.
        * @param    Where to display, likely System.out or System.err.
        * @return     <code>int</code> The length of the string.
        */
        public static long writeline (String message, PrintStream stream) {
	    //	    if (stream == System.err){
	    //	stream.println(message);
	    //}
	    stream.print(message);
	    return message.length();
        }


	public static void main( String[] args ) {


		writeline ("Zdravei Sviat", System.err);
		newline(System.err);
		writeline ("Hello World", System.out);
		newline(System.out);
		decout (123, System.out);
		newline(System.out);
		decout (0, System.out);
		newline(System.out);
		hexout (0xFEEDDAD, System.out);
		newline(System.out);
	}
}
