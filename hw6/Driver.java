/****************************************************************************
                                                                              
                                                Aaron Hernandez               
                                                CSE 12, Spring 2017           
                                                May 10, 2017                  
                                                cs12xcb                       
                              Assignment 6                                    
                                                                              
File Name:      Driver.java
Description:    This program has the UCSDStident class which sets has         
                constructor, deconstuctor, setters and getter methods.        
                Then it drives the table by checking input and calls          
                methods accordingly.                                          
                                                                              
****************************************************************************/
import java.io.EOFException;

//=========================================================================
// class UCSDStudent
//
// Description: Holds the info for a UCSD student including their name and
//              number. Can assign a number and get the name. Also sets the
//              hashcode and can check if another class object is equal to
//              it. Lastly it has a toSting method.
//
// Data Fields:
//     name (Sting)  - the student's name
//     Studentnum (long)  - the student number
//
// Public functions:
//     UCSDStudent - Constructor for the class to set values
//     getName - returns the student's name
//     equals - checks if the class object is equal to this object
//     assign - assigns a student's number
//     hashCode - gets the ASCII sum of the students's name
//     toString - prints a formated string
//==========================================================================
class UCSDStudent extends Base {
        private String name;
        private long Studentnum;

        /**                                                                                                                                                  
	 * formats the string to be printed                                                                                                                  
	 *                                                                                                                                                   
	 * @return  the formated sting                                                                                                                       
	 */
        public String toString () {

                return "name: " + name + " Studentnum: " + Studentnum;
        }

        /**
	 *Constuctor that sets the values of student
	 *
	 *@param string name of student
	 *@param Student's number to be set
	 */ 
        public UCSDStudent(String nm, long val) {  
                name = new String (nm);  
                Studentnum = val;  
        }

        /**                                                                                                                                                  
	 *Constuctor that sets the values of student                                                                                                         
	 *                                                                                                                                              
	 *@param Student object                                                                                                            
	 */
        public UCSDStudent(UCSDStudent UCSDStudent) {  
                name = new String (UCSDStudent.name);  
                Studentnum = UCSDStudent.Studentnum;
        }
        
        /**                                                                                                                                                 
	 * Gets the name of the student
	 *                                                                                                                  
	 * @return  the student's name                                                                                                    
	 */
        public String getName() {  
                return name;  
        } 

        /**                                                                                                                                                
	 * Checks if the name is of the other object is the same as this object                               
	 *                                                                                                                                                   
	 * @param   the other student object
	 * @return  true or false if it is equal or not                                                                                                 
	 */
        public boolean equals (Object other) {  
                if (this == other)   
                        return true;  
                  
                if (!(other instanceof UCSDStudent))   
                        return false;  
                UCSDStudent otherStu = (UCSDStudent) other;  
                  
                return name.equals (otherStu.getName ());  
        } 

        /**
	 * This method determines the ASCII sum of the student name
	 *
	 * @return  the ASCII sum
	 */
        public int hashCode () {  
	    int retval = 0;  
	    int index = 0;  
	    
	    //loop that adds up the ASCII sum
	    while (index != name.length ()) {  
		retval += name.charAt (index);  
		index ++;  
	    }  
	    
	    return retval;  
        }  
  
        /**                                                                                                                                                 
	 * Assigns a number to students number                                                                                          
	 *
	 * @param   the number to assign                                                                                                           
	 * @return  a new student with the number assigned                                                                                
	 */
        public UCSDStudent assign (long val) {  
                  
                UCSDStudent retval;        // return value  
  
                // give variable its value  
                Studentnum = val;  
                retval = new UCSDStudent (this);  
  
                return retval;  
        }
}

public class Driver {

        public static void main (String [] args) {

                /* initialize debug states */
                HashTable.debugOff();

                /* check command line options */
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                HashTable.debugOn();
                }

                /* The real start of the code */
                SymTab symtab = new SymTab (5);
                String buffer = null;
                char command;
                long number = 0;

                System.out.print ("Initial Symbol Table:\n" + symtab);

                while (true) {
                        command = 0;    // reset command each time in loop
                        System.out.print ("Please enter a command:  "
                                + "((i)nsert, (l)ookup, (w)rite):  ");

                        try {
                        command = MyLib.getchar ();
                        MyLib.clrbuf (command); // get rid of return

                        switch (command) {
                        case 'i':
                                System.out.print (
                                "Please enter UCSD Student name to insert:  ");
                                buffer = MyLib.getline ();// formatted input

                                System.out.print (
                                        "Please enter UCSD Student number:  ");

                                number = MyLib.decin ();
                                MyLib.clrbuf (command); // get rid of return

                                // create Student and place in symbol table
                                if(!symtab.insert (
                                        new UCSDStudent (buffer, number), 1)){

                                        System.out.println("Couldn't insert " + 
                                                           "student!!!"); 
                                }
                                break;

                        case 'l': {
                                Base found;     // whether found or not

                                System.out.print (
                                "Please enter UCSD Student name to lookup:  ");

                                buffer = MyLib.getline ();// formatted input

                                UCSDStudent stu = new UCSDStudent (buffer, 0);
                                found = symtab.lookup (stu);
                                
                                if (found != null) {
                                        System.out.println ("Student found!!!");
                                        System.out.println (found);
                                }
                                else
                                        System.out.println ("Student " + buffer
                                                + " not there!");
                                }
                                break;

                        case 'w':
                                System.out.print (
                                    "The Symbol Table contains:\n" + symtab);
                        }
                        }
                        catch (EOFException eof) {
                                break;
                        }
                }

                System.out.print ("\nFinal Symbol Table:\n" + symtab);
        }
}
