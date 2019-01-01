/* file: fileReader.cpp
 * 	COMMENT: MUST rewrite this code so that it behaves like
 * 	"std::strtok" that has state info, that keeps delivering new
 * 	lines until NULL is returned.
 *
 * 	In fileProcessor.cpp, we used "strtok" to process line-based text files:
 * 		-- ignoring initial white space
 * 		-- ignoring terminal white space
 * 		-- replacing multiple white spaces to one space
 * 		-- ignoring "#" token and rest of line
 *
 * 		CAREFUL:
 * 		This "#" MUST be a separate token!
 * 		E.g., "abc# xyz" and "abc #xyz" and "abc#xyz" do not count.
 * 	
 * 	Now, we extend the processor with:
 * 		-- removing the token "\\" of one charactor, provided it is 
 * 			the last token in the current line, and join
 * 			the following line to the current.
 * 		CAREFUL:
 * 		This "\\" MUST be a separate token.
 * 		E.g., "abc\\" does not count because "\\" is not separate.
 * 		E.g., "abc \\ xyz" does not count as "\\" is not the last token
 *
 * 	It will take an input file "example.txt"
 * 	and produce a cleaned up output file, "example-clean.txt".
 *
 * 	USAGE:
 * 		> fileReader
 *
 * 	KNOWN (SMALL) BUG:
 * 		If the last token is not "\\", we output an empty last line.
 * 		Otherwise, we do not output any empty last line.
 *
 * REFERENCES:
 * 	For file I/O, see fileStreamIO.cpp (this directory).
 * 	For strtok, see strtok.cpp (this directory).
 *
 * 	Author: Chee Yap (April 2012)
 *
 * ***************************************************/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "fileReader.h"

using namespace std;

/* *************************************************** 
 * MAIN PROGRAM
 *************************************************** */
int main (void) {
  //////////////////////////////////////////////////
  // Create sample input file
  sampleFile("example.txt");

  //////////////////////////////////////////////////
  // Create fileReader instance
  fileReader myReader("example.txt");

  //////////////////////////////////////////////////
  // Do the reading main loop:
  
  while (myReader.nextOutputLine())
      cout << "outputLine: " << myReader.outputLine << endl;

  validate();
  return 0;
}

