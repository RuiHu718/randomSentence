/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

/*************************************
 TODO:
   The current version works but it is a hack
   the token scanner + isNonternimal logic + puctuation marks
   are causing a lot of problem. 
   For example: "<plea>." is considered a key when "<plea>" is
   also, <abc is considered key when <abc-def> is
   The final result also needs to improve, need to remove the unnecessary spaces
 *************************************/


#include <iostream>
#include <fstream>
#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim
#include "map.h"
#include "vector.h"
#include "random.h"
#include "tokenscanner.h"

using namespace std;

static const string kGrammarsDirectory = "grammars/";
static const string kGrammarFileExtension = ".g";

void loadGrammar(string filaname, Map<string, Vector<string> > &grammarMap);
string randomPick(Vector<string> &vec);
bool hasNonTerminal(string s);
string writeSentence(Map<string, Vector<string> > &grammarMap, string key);

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

    // for (string s : grammarMap) {
    //   cout << s << " : " ;
    //   cout << grammarMap[s].toString() << endl;
    // }

    // cout << randomPick(grammarMap["<plea>."]) << endl;

    // if (hasNonTerminal("this is a test")) {
    //   cout << "true" << endl;
    // } else {
    //   cout << "false" << endl;
    // }

    cout << writeSentence(grammarMap, "<start>") << endl;
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
  string currentKey;
  int flag = 0;			// indicates beginning of expansion section

  while (true) {
    getLine(infile, line);
    if (infile.fail()) {
      break;
    }

    // when see a number, start adding
    // to vectors
    if (stringIsInteger(line)) {
      flag = 1;
      continue;
    }
    // when see a blank line, finish
    // adding to vectors
    if (line == "") {
      flag = 0;
      continue;
    }

    if (flag == 1) {
      //parse line into tokens and add them to vectors
      //cout << "add to vector" << endl;
      Vector<string> temp = grammarMap.get(currentKey);
      temp.add(line);
      grammarMap.put(currentKey, temp);
    } else {
      //cout << "add to map" << endl;
      currentKey = line;
      grammarMap.get(currentKey);
    }
  }
}


/* Function: randomPick
 * Usage:    string s = randomPick(vector)
 * ---------------------------------------
 * Precondition:
 * Postcondition:
 */
string randomPick(Vector<string> &vec) {
  int size = vec.size();
  int i = randomInteger(0, size - 1);
  return vec[i];
}


/* Function: hasNonTerminal
 */
bool hasNonTerminal(string s) {
  TokenScanner scanner(s);
  scanner.ignoreWhitespace();
  //need to add more punctuations here
  scanner.addWordCharacters("<");
  scanner.addWordCharacters(">");
  scanner.addWordCharacters("(");
  scanner.addWordCharacters(")");  
  scanner.addWordCharacters(",");
  scanner.addWordCharacters(".");
  scanner.addWordCharacters("!");
  scanner.addWordCharacters("?");
  scanner.addWordCharacters("'");      
  
  while (scanner.hasMoreTokens()) {
    string token = scanner.nextToken();
    if (startsWith(token, "<")) return true;
  }

  return false;
}


string writeSentence(Map<string, Vector<string> > &grammarMap, string key) {
  string current = randomPick(grammarMap[key]);

  // base case
  if (!hasNonTerminal(current)) {
    return current;
  } else {
    string result;
    // this part is duplicated, should be moved to a seperate function
    TokenScanner scanner(current);
    scanner.ignoreWhitespace();
    //need to add more punctuations here
    scanner.addWordCharacters("<");
    scanner.addWordCharacters(">");	
    scanner.addWordCharacters("-");			     
    // scanner.addWordCharacters("(");
    // scanner.addWordCharacters(")");  
    // scanner.addWordCharacters(",");
    // scanner.addWordCharacters(".");
    // scanner.addWordCharacters("!");
    // scanner.addWordCharacters("?");
    // scanner.addWordCharacters("'");      
  
    while (scanner.hasMoreTokens()) {
      string token = scanner.nextToken();
      if (!startsWith(token, "<")) {
	result = result + " " + token;
      } else {
	cout << "calling token: " << token << endl;
	result = result + " " + writeSentence(grammarMap, token);
      }
    }
    // should parse the result one more time to remove unnecessary spaces
    return result;
  }
}
