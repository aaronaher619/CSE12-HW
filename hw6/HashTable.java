/****************************************************************************

                                                Aaron Hernandez
                                                CSE 12, Spring 2017
                                                May 10, 2017
                                                cs12xcb
                              Assignment 6

File Name:      HashTable.java
Description:    This program creates a new table. Then you can insert or 
                lookup elements from the table which calls locate. Can also 
                check for debug flag and has a write function.

****************************************************************************/
public class HashTable extends Base {

	private static int counter = 0;	// number of HashTables so far
        private static boolean debug;        // allocation of debug state

        // set in locate, last location checked in hash table 
        private int index = 0; 

        // set in insert/lookup, count of location in probe sequence
        private int count = 0;
        
        private int probeCount[];   // where we are in the probe sequence 
        private long occupancy;     // how many elements are in the Hash Table
        private int size;           // size of Hash Table
        private Base table[];       // the Hash Table itself ==> array of Base
        private int tableCount;     // which hash table it is

        // messages
        private static final String AND = " and ";
        private static final String DEBUG_ALLOCATE = " - Allocated]\n";
        private static final String DEBUG_LOCATE = " - Locate]\n";
        private static final String DEBUG_LOOKUP = " - Lookup]\n";
        private static final String BUMP = "[Bumping To Next Location...]\n";
        private static final String COMPARE = " - Comparing ";
        private static final String FOUND_SPOT = " - Found Empty Spot]\n";
        private static final String HASH = "[Hash Table ";
	private static final String HASH_VAL = "[Hash Value Is ";
        private static final String INSERT = " - Inserting ";
        private static final String PROCESSING = "[Processing ";
        private static final String TRYING = "[Trying Index ";

        
        /**
         *This method turns debug on.
         */
        public static void debugOn () {
	    debug = true;
        }

        /**
         *This method turns debug off.
	 */  
        public static void debugOff () {
            debug = false;
        }

        /**
         * This methos allocates and initializes the memory
         * associated with a hash table.
         *
         * @param  sz   The number of elements for the table...MUST BE PRIME!!!
         */
        public HashTable (int sz) {

	    /*Initializes memory*/
	    table = new Base[sz];
            probeCount = new int[sz];
	    ++counter;
	    occupancy = 0;
	    size = sz;
	    tableCount = counter;
	    
	    /*Prints Debug message*/
	    if(debug){
		System.err.print(HASH + counter + DEBUG_ALLOCATE);
	    }
        }


        /**
         * This method will insert the element in the hash table.
         * If the element cannot be inserted, false will be returned.
         * If the element can be inserted, the element is inserted
         * and true is returned.  Duplicate insertions will
         * cause the existing element to be deleted, and the duplicate
         * element to take its place.
         *
         * @param   element       The element to insert.
         * @param   initialCount  Where to start in probe seq (recursive calls) 
         * @return  true or false indicating success or failure of insertion
         */
        public boolean insert (Base element, int initialCount){
	    /*Declares the bumped values*/
	    Base bumped;
	    int bumpedCount;
	    
	    /*Checks if element is null*/
	    if (element == null){
		return false;
	    }
	    
	    //Checks for Debug
            if(debug){
                System.err.print(HASH + counter + INSERT + element.getName() + "]\n");
		System.err.print(HASH + counter + DEBUG_LOCATE);
            }

	    /*Sets count and calls locate to set index*/
	    count = initialCount;
	    if (!(locate(element))){
		return false;
	    }

	    /*Uses index to initialize the bumped values*/ 
	    bumped = table[index];
	    bumpedCount = probeCount[index];

	    /*inserts the element in the table at the index*/
	    table[index] = element;
            probeCount[index] = count;

	    /*If an element needs to be bumped then it recursively calls it*/
	    if((bumpedCount < count) && (count > 1)){

		//Checks for Debug
		if(debug){System.err.print(BUMP);}

                insert(bumped, bumpedCount);
	    }
    
	    /*Returns success*/
	    return true;
        }


        /**
         * This method will locate the location in the
         * table for the insert or lookup.
         *
         * @param   element  The element needing a location.
         * @return  true if tem found, or false if not found
         */
        private boolean locate (Base element) {
	    int newIndex;
	    int sum = element.hashCode();
	    int initialLoc = sum % size;
	    int inc = sum % (size - 1) + 1;
	    newIndex = initialLoc;

	    //Checks for Debug
            if(debug){
                System.err.print(PROCESSING + element.getName() + "]\n");
		System.err.print(HASH_VAL + sum + "]\n");
            }

	    //Main loop that sets index
	    while(true){

		//Checks for Debug
		if(debug){
		    System.err.print(TRYING + newIndex + "]\n");
		}

		//checks if the index is empty
		if(table[newIndex] == null){

		    //Checks for Debug
		    if(debug){
			System.err.print(HASH + counter + FOUND_SPOT);
		    }

		    index = newIndex;
		    ++occupancy;
		    return true;
		}
		
		//Checks for Debug
		if(debug){
		    System.err.print(HASH + counter + COMPARE + element.getName() + AND + table[newIndex].getName() + "]\n");
		}

		//Checks if the element is equal to the element at the index
		if(element.equals(table[newIndex])){
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


        /**
         * This method will lookup the element in the hash table.  If
         * found a pointer to the element is returned.  If the element
         * is not found, NULL will be returned to the user.
         *
         * @param   element  The element to look up.
         * @return  A pointer to the element if found, else NULL
         */
        public Base lookup (Base element) {
	    //Checks for Debug
	    if(debug){
		System.err.print(HASH + counter + DEBUG_LOOKUP);
	    }

	    //Sets the count to 1 and uses locate to find if element is in the table
	    count = 1;
	    if(locate(element)){
		
		//Checks for debug
		if(debug){System.err.print(HASH + counter + DEBUG_LOCATE);}

		//Checks if index of table is empty
		if(table[index] == null){
		    return null;
		}

		//Checks if item hasn't traveled as long
		if(probeCount[index] < count){
		    return null;
		}     	

		//return the found element
		return table[index];
	    }
	    return null;
        }


        /**
         * Creates a string representation of the hash table. The method 
         * traverses the entire table, adding elements one by one ordered
         * according to their index in the table. 
         *
         * @return  String representation of hash table
         */
        public String toString () {
                String string = "Hash Table " + tableCount + ":\n";
                string += "size is " + size + " elements, "; 
                string += "occupancy is " + occupancy + " elements.\n";

                /* go through all table elements */
                for (int index = 0; index < size; index++) {

                        if (table[index] != null) {
                                string += "at index " + index + ": ";
                                string += "" + table[index];
                                string += " with probeCount: "; 
                                string += probeCount[index] + "\n";
                        }
                }

                return string;
        }
}
