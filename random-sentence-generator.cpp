/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <iostream>
#include <fstream>
#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim
#include "map.h"
#include "vector.h"

using namespace std;

static const string kGrammarsDirectory = "grammars/";
static const string kGrammarFileExtension = ".g";

void loadGrammar(string filaname, Map<string, Vector<string> > &grammarMap);

static string getNormalizedFilename(string filename) {
  string normalizedFileName = kGrammarsDirectory + filename;
  if (!endsWith(normalizedFileName, kGrammarFileExtension))
    normalizedFileName += kGrammarFileExtension;
  return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
  string normalizedFileName = getNormalizedFilename(filename);
  ifstream infile(normalizedFileName.c_str());
  return !infile.fail();
  //return true;
}

static string getFileName() {
  while (true) {
    string filename = trim(getLine("Name of grammar file? [<return> to quit]: "));
    if (filename.empty() || isValidGrammarFilename(filename)) return filename;
    cout << "Failed to open the grammar file named \"" << filename << "\". Please try again...." << endl;
  }
}

int main() {
  while (true) {
    string filename = getFileName();
    if (filename.empty()) break;
    cout << "Here's where you read in the \"" << filename << "\" grammar "
	 << "and generate three random sentences." << endl;

    Map<string, Vector<string> > grammarMap;
    loadGrammar(filename, grammarMap);
    
  }
    
  cout << "Thanks for playing!" << endl;
  return 0;
}


/* Function loadGrammar
 * Usage:   loadGrammar(filename, grammarMap)
 * ------------------------------------------
 * Parses the grammar file, load it into a map of string to vector
 * Keys are nonterminals, values are all the possible extentions of
 * That nonterminal
 * Precondition:
 * Postcondition:
 */
void loadGrammar(string filename, Map<string, Vector<string> > &grammarMap) {

  ifstream infile;
  infile.open(filename.c_str());
  string line;
  int flag = 0;			// indicates beginning of expansion section

  while (true) {
    getLine(infile, line);
    if (infile.fail()) {
      break;
    }

    if (stringIsInteger(line)) {
      flag = 1;
      continue;
    }
    if (line == "") {
      flag = 0;
      continue;
    }

    if (flag == 1) {
      //parse line into tokens and add them to vectors
      cout << "add to vector" << endl;
    } else {
      cout << "add to map" << endl;
    }
  }
}
