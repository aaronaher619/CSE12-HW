/****************************************************************************   
                                                                                
                                                Aaron Hernandez                 
                                                CSE 12, Spring 2017             
                                                May 17, 2017                    
                                                cs12xcb                         
                              Assignment76                                      
                                                                                
File Name:      Driver.java                                                     
Description:    This program has the UCSDStident class which sets has           
                constructor, deconstuctor, setters and getter methods.          
                Then it drives the tree by checking input and calls            
                methods accordingly.                                            
                                                                                
****************************************************************************/
import java.io.*;

//=========================================================================
// Class UCSDStudent extends base
//
// Description: Implements a UCSDStudent object class to be used by the tree
//              When inserting objects
//
// Data Fields:
//     name (Sting)  - Student's name
//     studentnum (Node *)  - Student's number
//
// Public functions:
//     UCSDStudent - constuctor
//     getName - return name of student
//     equals - return true if the names of both object are equal
//     isGreaterThan - if  a name is greater than thestudents name
//     toSting - pring a formated student object
//
//==========================================================================
class UCSDStudent extends Base {
    private String name;
    private long studentnum;
    
    /*
     * UCSDStudent Constuctor
     *
     * @param nm - Student's name
     * @param val - student's number
     */
    public  UCSDStudent (String nm, long val) {  
        name = new String (nm);  
        studentnum = val;  
    }  

    /*
     * Gets the name of the student
     *
     * @return string of student's name
     */    
    public String getName () {  
        return name;  
    }  

    /*
     * if two students are equal
     *
     * @param object - A object to compare student to
     * @return true or false
     */
    public boolean equals (Object object) {  
        if (this == object){  
            return true;  
	}
	
        if (!(object instanceof UCSDStudent)){  
            return false;  
	}
	
        UCSDStudent otherStu = (UCSDStudent) object;  
	
        return name.equals(otherStu.getName ());  
    }  

    /*
     * If one student is greater than another
     *
     * @param base - the base object
     * @return true of false
     */
    public boolean isGreaterThan (Base base) {  
        return (name.compareTo(base.getName ()) > 0) ? true : false;  
    }
    
    /*
     * Print a nice formated student
     *
     * @return formated student string
     */
    public String toString () {
	return "name:  " + name + "  studentnum:  " + studentnum;
    }
}

public class Driver {
	private static final short NULL = 0;

	public static void main (String [] args) {

		/* initialize debug states */
		Tree.debugOff();

		/* check command line options */
		for (int index = 0; index < args.length; ++index) {
			if (args[index].equals("-x"))
				Tree.debugOn();
		}


		/* The real start of the code */
		SymTab<UCSDStudent> symtab = 
			new SymTab<UCSDStudent>("UCSDStudentTree");
		String buffer = null;
		char command;
		long number = 0;

		System.out.println ("Initial Symbol Table:\n" + symtab);

		while (true) {
			command = NULL; // reset command each time in loop
			System.out.print ("Please enter a command:  " + 
					"((a)llocate, (i)nsert, (l)ookup, " +
					"(r)emove, (w)rite):  ");

			try {
				command = MyLib.getchar ();
				MyLib.clrbuf (command); // get rid of return

				switch (command) {
				case 'a':
					System.out.print
						("Please enter name of new " + 
						"Tree to allocate:  ");

					buffer = MyLib.getline ();// formatted input
					symtab = new SymTab<UCSDStudent>(buffer);
					break;

				case 'i':
					System.out.print
						("Please enter UCSD student name to insert:  ");

					buffer = MyLib.getline ();// formatted input

					System.out.print
						("Please enter UCSD student number:  ");

					number = MyLib.decin ();
					MyLib.clrbuf (command); // get rid of return

					// create student and place in symbol table
					symtab.insert(new UCSDStudent (buffer, number));
					break;

				case 'l': { 
					UCSDStudent found;      // whether found or not

					System.out.print
						("Please enter UCSD student name to lookup:  ");
					buffer = MyLib.getline ();// formatted input

					UCSDStudent stu = new UCSDStudent (buffer, 0);
					found = symtab.lookup (stu);

					if (found != null) {
						System.out.println("Student found!");
						System.out.println(found);
					}
					else
						System.out.println 
							("student " + buffer
							  + " not there!");
					}
					break;

				case 'r': { 
					UCSDStudent removed; // data to be removed

					System.out.print
						("Please enter UCSD student" + 
						 " name to remove:  ");

					buffer = MyLib.getline ();

					UCSDStudent stu = new UCSDStudent (buffer, 0);

					removed = symtab.remove(stu);

					if (removed != null) {
						System.out.println("Student removed!"); 
						System.out.println(removed);
					}
					else
						System.out.println 
							("student " + buffer
							+ " not there!");
				}
				break;

				case 'w':
					System.out.println("The Symbol Table " +
							"contains:\n" + symtab);
				}
			}
			catch (EOFException eof) {
				break;
			}
		}
		System.out.println("\nFinal Symbol Table:\n" + symtab);
	}
}
