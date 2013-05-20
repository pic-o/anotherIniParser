/***********************************************************************************************************************************************************
Original Web Link:
http://blog.pic-o.com/2013/05/another-ini-parser/

Personal request:
Drop me an email if you do use this / encounter problems =)

License Type:
BSD 3-Clause (This is both "GPL" and "commercial with acknowledgement" compatible)

Copyright (c) 2013, picoCreator (Eugene Cheah Hui Qin | pico.creator{at}gmail.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

+ Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
+ Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
+ Neither the name of ANY ORGANIZATION(S) AFFLIATED WITH THE CREATOR (picoCreator) nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********************************************************************************************************************************************************/
#include "iniParser.hpp"
#include "charTools.hpp"

//Macro defines if _charMacros are not already used
#ifndef _charMacros
#define CHAR_CR 13
#define CHAR_LF 10
#define CHAR_TAB 9
#define CHAR_SPACE 32
#define IS_CHAR_CR(chr) (chr == CHAR_CR)
#define IS_CHAR_LF(chr) (chr == CHAR_LF)
#define IS_CHAR_TAB(chr) (chr == CHAR_TAB)
#define IS_CHAR_SPACE(chr) (chr == CHAR_SPACE)

#define CHAR_EQUAL 61 //=
#define CHAR_NUMSIGN 35 //#
#define CHAR_SEMICOLON 59 //;
#define CHAR_LEFT_BRACKET 91 //[
#define CHAR_RIGHT_BRACKET 93 //]

#define IS_CHAR_EQUAL(chr) (chr == CHAR_EQUAL)
#define IS_CHAR_NUMSIGN(chr) (chr == CHAR_NUMSIGN)
#define IS_CHAR_SEMICOLON(chr) (chr == CHAR_SEMICOLON)
#define IS_CHAR_LEFT_BRACKET(chr) (chr == CHAR_LEFT_BRACKET)
#define IS_CHAR_RIGHT_BRACKET(chr) (chr == CHAR_RIGHT_BRACKET)
#endif

//Cross platform (X) file locking option
#ifdef __linux__ //linux style flock
	#include <sys/file.h>
	#define X_FILENO(file) fileno(file)
	#define X_FILE_SHARELOCK(file,fileNo) flock(fileNo, LOCK_SH)
	#define X_FILE_UNLOCK(file,fileNo) flock(fileNo, LOCK_UN);
#else 
	#ifdef _WIN32 //windows style : locks/unlock first byte
		#include <windows.h>
		#define X_FILENO(file) 0
		#define X_FILE_SHARELOCK(file,fileNo) LockFile(file, 0,0, 1,0)
		#define X_FILE_UNLOCK(file,fileNo) UnlockFile(file, 0,0, 1,0)
	#else
		#ifdef __APPLE__ //Apple is based on linux flock : i think
			#include <sys/file.h>
			#define X_FILENO(file) fileno(file)
			#define X_FILE_SHARELOCK(file,fileNo) flock(fileNo, LOCK_SH)
			#define X_FILE_UNLOCK(file,fileNo) flock(fileNo, LOCK_UN);	
			
			#error MAC file locking support is currently UNTESTED. comment this line if this is intentional  
		#else
			#error Compilling for non-supported platform  
		#endif
	#endif
#endif

/************************************************************************************
*	Class: iniParser
*	Provides a small, simple, high performace ini config file parser
************************************************************************************/

/**
* Function: sectionMap
* Gets and returns the section variable map, if it exists. Else it creates one if applicable.
* 
* Returns:
* std::map<char*,char*,charTools::cmpo_less> * section map point holding the variables data
*
* Parameters:
* sectionName - the input char * of the section name to be fetched (Null defaults as "")
* generates - set to true, to indicate that the section is to be created if it currently does not exists.
**/
std::map<char*,char*,charTools::cmpo_less> * iniParser::sectionMap( const char * sectionName, bool generates ) {
	// const_cast<char*> is a needed evil, as map::find does not accept the const despite not modifying it
	char* assumedName = (sectionName? const_cast<char*>(sectionName) : (char*)"");
	
	std::map<char*, std::map<char*,char*,charTools::cmpo_less> *, charTools::cmpo_less >::iterator sectMap;
	sectMap = storedConfig.find( assumedName ); //searches the configuration map
	if( sectMap == storedConfig.end() ) { //great it does not exists
		if( generates == true ) {
			//So we register the new section
			char * storeName = new char[ strlen(assumedName) + 1 ];
			storeName[0] = 0; //safety
			strcpy( storeName, assumedName ); //copies the values over
			
			std::map<char*,char*,charTools::cmpo_less> * blankSect = new std::map<char*,char*,charTools::cmpo_less>();
			storedConfig.insert( 
				std::pair < char*, std::map<char*,char*,charTools::cmpo_less> * >
				( storeName, blankSect ) 
			);
			return blankSect;
		} else {
			return NULL;
		}
	} //else
	return sectMap->second;
}

/**
* Function: sectionCount
* Count the number of sections =)
*
* Returns:
* The number of sections created in the parser, including the null blank ones
**/
int iniParser::sectionCount() {
	return (int)storedConfig.size();
}

/**
* Function: sectionValueGet
* Fetches the "name"="value" value. This returns the raw stored pointer, and should not be edited. See <iniParser._sectionValueSet>
*
* Returns:
* The stored char * pointer value
*
* Parameters:
* sectionName - The section name which the value belongs to.
* valueName - "name"
**/
char * iniParser::sectionValueGet( const char * sectionName, const char * valueName ) {
	std::map<char*,char*,charTools::cmpo_less> * map = sectionMap(sectionName);
	if( map != NULL ) {
		return sectionValueGet( map, valueName );
	}
	return NULL;
}

/**
* Function: sectionValueGet
* Fetches the "name"="value" value. This returns the raw stored pointer, and should not be edited. See <iniParser._sectionValueSet>
*
* Returns:
* The stored char * pointer value
*
* Parameters:
* map - the section map, as returned via <iniParser.sectionMap>
* valueName - "name"
**/
char * iniParser::sectionValueGet( std::map<char*,char*,charTools::cmpo_less> * map, const char * valueName ) {
	std::map<char*, char*, charTools::cmpo_less>::iterator valueMap;
	
	//finding the right "name=value" pair
	//const_cast<char*> is a needed evil, as map::find does not accept the const despite not modifying it
	valueMap = (*map).find( const_cast<char*>(valueName) );
	if( valueMap == (*map).end() ) { //great it does not exists
		return NULL;
	} else { //return existing pair
		char* retVal = valueMap->second;
		int maxLen = strlen(retVal);
		for(int pt=0; pt<maxLen; ++pt) {
			if( isspace(retVal[pt]) == 0 ) { //none whitespace : found it, and returns
				return retVal;
			}
		}
		return NULL; //its pure white space? : return NULL
	}	
}

/**
* Function: sectionValueSet
* Create copies and stores the "name"="value" pair in the section map safely. See the faster unsafe version <iniParser._sectionValueSet> for more information.
*
* Returns:
* return true if value is set, else false (generates = false & section does not exists)
*
* Parameters:
* sectionName - The section name which the value belongs to.
* valueName - "name"
* valueData - "value"
* generates - Indicates if the section is to be created if it does not exists (default as true)
**/
bool iniParser::sectionValueSet( const char * sectionName, const char * valueName, const char * valueData, bool generates ) {
	std::map<char*,char*,charTools::cmpo_less> * map = sectionMap(sectionName, generates);
	if( map != NULL ) {
		sectionValueSet( map, valueName, valueData );
		return true;
	}
	return false;
}

/**
* Function: sectionValueSet
* Create copies and stores the "name"="value" pair in the section map safely. See the faster unsafe version <iniParser._sectionValueSet> for more information.
*
* Parameters:
* map - the section map, as returned via <iniParser.sectionMap>
* valueName - "name"
* valueData - "value"
**/
void iniParser::sectionValueSet( std::map<char*,char*,charTools::cmpo_less> * map, const char * valueName, const char * valueData ) {
	//safe storage =)
	char * storeName = new char[ strlen(valueName) + 1 ];
	storeName[0] = 0; //safety
	strcpy( storeName, valueName ); //copies the value over
	
	char * storeData = new char[ strlen(valueData) + 1 ];
	storeData[0] = 0; //safety
	strcpy( storeData, valueData ); //copies the value over
	
	_sectionValueSet( map, storeName, storeData );
}

/**
* Function: _sectionValueSet
* Stores the "name"="value" pair in the section map directly.
*
* Warning!:
* The pointers storeName/storeData memory address is used and stored directly, thus they must be created via the "new char[?]" statement. 
* Else unpredictable reasults due to memory corruption may occur, if temporary function variables were used (hopefully fatal crash, it can be worse).
* For the safer version, which creates safe copies of the variables passed before storing, use <iniParser.sectionValueSet>.
*
* Note:
* The destruction of these char * variabels will also automatically be done via <iniParser.clear> which is also called durring class destruction <iniParser.~iniParser>.
*
* Parameters:
* map - the section map, as returned via <iniParser.sectionMap>
* storeName - "name", this value is deleted if it already exits
* storeData - "value"
**/
void iniParser::_sectionValueSet( std::map<char*,char*,charTools::cmpo_less> * map, char * storeName, char * storeData ) {
	std::map<char*, char*, charTools::cmpo_less>::iterator valueMap;
	
	//finding the right "name=value" pair
	valueMap = (*map).find(storeName);
	if( valueMap == (*map).end() ) { //great it does not exists
		//So we register a new pair
		(*map).insert(
			std::pair < char*, char* > ( storeName, storeData )
		);	
	} else { //we overide the existing pair value
		//updates an existing pair
		delete( (*valueMap).second ); //deletes old values
		(*valueMap).second = storeData; //sets new value
		delete(storeName);
	}
}

/**
* Function: sectionValueCount
* The current number of stored values whitin a section
*
* Parameters:
* map - the section map, as returned via <iniParser.sectionMap>
**/
int iniParser::sectionValueCount( std::map<char*,char*,charTools::cmpo_less> * map ) {
	return (int)(*map).size();
}

/**
* Function: sectionValueCount
* The current number of stored values whitin a section
*
* Returns:
* -1 if the section does not exists, else returns its current size (0~N)
*
* Parameters:
* sectionName - The section name which the value belongs to.
**/
int iniParser::sectionValueCount( const char * sectionName ) {
	std::map<char*,char*,charTools::cmpo_less> * map = sectionMap(sectionName);
	if( map != NULL ) {
		return sectionValueCount( map );
	}
	return -1;
}



/**
* Function: parseConfigSegment
* [Private] Core function, that does the processing of ini configs, in segments, or in complete sets.
* 
* Returns:
* The point after the last successfully parsed character in the string
*
* Parameters:
* inputStr - the input char * string to be parsed
* curSect - current section being parsed
**/
long iniParser::_parseConfigSegment( const char * inputStr, std::map<char*,char*,charTools::cmpo_less> *& curSect ) {

	long chkPt = 0;
	long pt = 0;
	int a, b, c, d, e;
	
	char * valueData = NULL;
	char * valueName = NULL;
	
	std::map<char*, std::map<char*,char*,charTools::cmpo_less>, charTools::cmpo_less >::iterator sectMap;
	std::map<char*, char*, charTools::cmpo_less>::iterator valueMap;
	
	//'#' / ';' represents comment lines
	//'[' represents starting of a section
	
	while( true ) {
		////////////////////////////////////
		// parse state = new line / fresh //
		////////////////////////////////////
		
		//i know this is non std practise, but it make sense right? : Skipping useless starting spaces, tabs, and lines
		while( IS_CHAR_CR(inputStr[pt]) || IS_CHAR_LF(inputStr[pt]) || IS_CHAR_SPACE(inputStr[pt]) || IS_CHAR_TAB(inputStr[pt]) ) {
			//skips the starting blanks / blank lines and checks for end of field
			if(inputStr[++pt] == 0) { return pt; }
		} 
		chkPt = pt; //updates the chkPt pass the uneeded space / blank lines
		
		//This is the logic for processing comment lines
		if( IS_CHAR_NUMSIGN(inputStr[pt]) || IS_CHAR_SEMICOLON(inputStr[pt]) ) {
			////////////////////////////////
			// parse state = comment line //
			////////////////////////////////
			if(inputStr[++pt] == 0) { return pt; } //skips the comment line character : '#' / ';'
			
			//skips comment characters : #'any comment data blah blah'
			while( !(IS_CHAR_CR(inputStr[pt]) || IS_CHAR_LF(inputStr[pt])) ) {
				if(inputStr[++pt] == 0) { return pt; } //returns as valid if it reaches EOF
			}
			
			//successfully parses the comment line, registers the "chkPt"
			chkPt = pt;
		} else if( IS_CHAR_LEFT_BRACKET(inputStr[pt]) ) {
			////////////////////////////////
			// parse state = section name //
			////////////////////////////////
			if(inputStr[++pt] == 0) {  //'[' without trailing characters
				return chkPt; //returns the last valid parsed char
			}
			
			a = pt; //mark start of section name, after '['
			
			//skips the section name chars
			while(true) {
				if( IS_CHAR_RIGHT_BRACKET(inputStr[pt]) ) { //']'
					break;
				}
				
				if( IS_CHAR_CR(inputStr[pt]) || IS_CHAR_LF(inputStr[pt]) || inputStr[pt] == 0) { //invalid linebreaks
					return chkPt;
				}
				
				pt++;
			}
			b = pt; //the ']' point
			c = b - a; //the new section name length
			
			//skips any other irrevalent data till the new line is found
			while( !(IS_CHAR_CR(inputStr[pt]) || IS_CHAR_LF(inputStr[pt]) || inputStr[pt] == 0) ) {
				pt++;
			}
			
			valueName = new char[c + 1];
			//copy the data over to the current section status
			b = 0;
			for( ; b < c; b++ ) {
				valueName[b] = inputStr[a+b];
			}
			valueName[b] = 0; //str terminator
			
			curSect = sectionMap(valueName, true); //fetches or create the current section if needed
			delete( valueName );
			valueName = NULL;
			
			//successfully parses the section line, registers the "chkPt"
			chkPt = pt;
		} else { //since spaces / tabs / new lines / blank lines / comments / sections name should not ever reach here : assumes there might be data
			a = pt; //starting character in "name"=value
			while(true) {
				if( IS_CHAR_EQUAL(inputStr[pt]) ) { //'='
					b = pt; //ending character after "name"=value (should be '=' position)
					//skips the 'equal' value
					pt++;
					break;
				}
				
				if( IS_CHAR_CR(inputStr[pt]) || IS_CHAR_LF(inputStr[pt]) || inputStr[pt] == 0 ) { //linebreak -> no valide name=value line
					return chkPt;
				}
				
				pt++;
			}
			
			c = pt; //starting character for name="value"
			
			//skips till a new line is found //does nothing, if it is "already there"
			while( !(IS_CHAR_CR(inputStr[pt]) || IS_CHAR_LF(inputStr[pt]) || inputStr[pt] == 0) ) {
				pt++;
			}
			d = pt; //last character for name="value"
						
			//copy the data over to the current "name" inside valueName
			e = b - a; //gets the "name"=value length
			valueName = new char[ e + 1 ];
			b = 0;
			for( ; b < e; b++ ) {
				valueName[b] = inputStr[a+b];
			}
			valueName[b] = 0; //str terminator
			
			 
			//gets the value string
			e = d - c;
			valueData = new char[ e + 1 ];
			b = 0;
			for( ; b < e; b++ ) {
				valueData[b] = inputStr[c+b];
			}
			valueData[b] = 0;
			
			_sectionValueSet( curSect, valueName, valueData );
			
			//reset
			valueName = NULL;
			valueData = NULL; 
			
			//successfully / gracefully parses the name=value line, registers the "chkPt"
			chkPt = pt;
		}
	}
	
	return chkPt;
}
 
/**
* Function: parseIniStr
* Processes and parses a complete ini string.
* 
* Returns:
* 1 when successful, -(0~N) for the character point in the string where an error occured.
* 
* Parameters:
* inputStr - the input INI string to be parsed
* defSection - default section for stored values. (Defaults to NULL / "")
**/ 
long iniParser::parseIniStr( const char * inputStr, const char * defSection ) {
	std::map<char*,char*,charTools::cmpo_less> * curSect = sectionMap( defSection, true ); //default "blank"? section
	
	long iniChkPt = _parseConfigSegment( inputStr, curSect );
	
	if( iniChkPt < (long)strlen(inputStr) ) { //oh boy
		return -iniChkPt;
	}
	return 1;
}

/**
* Function: parseIniFile
* Processes and parses a complete ini file.
* 
* Returns:
* 1~N line number parsed when successful, -(1~N) for the line number in which an error occured
* 0 us returned when file does not exists / file access related error (not file content error)
* Parameters:
* filePath - the INI file to be parsed
* defSection - the section to parse the values as (Defaults to NULL / "")
* fileLocks - indicates if a fileLock is to be performed. (Default is true)
* lineBufferSize - set the maximum line size (default is 256)
**/ 
long iniParser::parseIniFile( const char * filePath, const char * defSection, bool fileLocks, int lineBufferSize ) {

	std::map<char*,char*,charTools::cmpo_less> * curSect = sectionMap( defSection, true ); //default "blank"? section
	
	FILE * fileHandle  = fopen( filePath, "r" ); //read only mode
	
	if( fileHandle != NULL ) {
		//long eofPt; //end of field point
		int lineNumber = 0, fileNum;
		
		if( fileLocks == true ) {
			fileNum = X_FILENO( fileHandle );
			X_FILE_SHARELOCK( fileHandle, fileNum );
		}
		
		if( 
		//	!(fseek( fileHandle, 0, SEEK_END ) == 0) || //eof position seek
		//	(eofPt = ftell( fileHandle )) == -1L ||  //eof position val
			!(fseek( fileHandle, 0, SEEK_SET ) == 0) //back to position
		) {
			if( fileLocks == true ) {
				X_FILE_UNLOCK(fileHandle,fileNum);
			} 
			fclose( fileHandle );
			return 0;
		}
		
		int lineLength;
		int lineBufferSizePlusOne = lineBufferSize + 1;
		char * fileData = new char [lineBufferSizePlusOne];
		fileData[0] = 0;
		
		while( fgets( fileData, lineBufferSizePlusOne, fileHandle ) != NULL ) {
			
			lineLength = strlen(fileData);
			if( lineLength >= lineBufferSize ) { //line is too long
				if( fileLocks == true ) {
					X_FILE_UNLOCK(fileHandle,fileNum);
				} 
				fclose( fileHandle );
				return -lineNumber;
			} //else
			
			if(lineLength != _parseConfigSegment(fileData, curSect)) { //line was not parsed properly
				if( fileLocks == true ) {
					X_FILE_UNLOCK(fileHandle,fileNum);
				} 
				fclose( fileHandle );
				return -lineNumber;
			}
			
			fileData[0] = 0; //resets it
			lineNumber++;
		}
		
		if( fileLocks == true ) {
			X_FILE_UNLOCK(fileHandle,fileNum);
		} 
		fclose( fileHandle );
		return lineNumber;
	}
	return 0;
}

/**
* Function: outputIniStr
* Outputs the entire ini dataset, into a single char string. Useful for dump debugging / writing ini files
**/
char* iniParser::outputIniStr() {
	std::map<char*, std::map<char*,char*,charTools::cmpo_less> *, charTools::cmpo_less >::iterator sectMap;
	std::map<char*, char*, charTools::cmpo_less>::iterator valueMap;
	long lenCount = 2;
	
	//itinerate over each section
	for ( sectMap=storedConfig.begin(); sectMap != storedConfig.end(); sectMap++ ) {
		lenCount += strlen((*sectMap).first);
		lenCount += 3; //account for the "[]" sign, "\n" and "\n" divisor between sections

		//itinerate over each value (inside a section)
		for( valueMap=(*((*sectMap).second)).begin(); valueMap != (*((*sectMap).second)).end(); valueMap++ ) {
			lenCount += strlen((*valueMap).first); //strlen the value name
			lenCount += strlen((*valueMap).second); //strlen the stored value
			lenCount += 2; //account for the "=" sign and "\n"
		}
	}

	char* retValue = new char[lenCount];
	retValue[0] = 0; //safety
	
	for ( sectMap=storedConfig.begin(); sectMap != storedConfig.end(); sectMap++ ) {
		if(sectMap!=storedConfig.begin()) {
			strcat(retValue, "\n"); 
		}

		strcat(retValue, (char*)"["); 
		strcat(retValue, (*sectMap).first); 
		strcat(retValue, (char*)"]\n");
		
		//itinerate over each value (inside a section)
		for( valueMap=(*((*sectMap).second)).begin(); valueMap != (*((*sectMap).second)).end(); valueMap++ ) {
			strcat(retValue, (*valueMap).first); 
			strcat(retValue, (char*)"="); 
			strcat(retValue, (*valueMap).second); 
			strcat(retValue, (char*)"\n"); 
		}
	}

	return retValue;
}

/**
* Function: sectionMapSet
* Clones the various "name=value" pairs into a specified section, from the map provided
*
* Returns:
* return true if value is set, else false (generates = false & section does not exists)
*
* Parameters:
* sectionName - The section name which the value belongs to.
* sourceMap - map source to copy values from
* generates - Indicates if the section is to be created if it does not exists (default as true)
**/
bool iniParser::sectionMapSet( const char* sectionName, std::map<char*,char*,charTools::cmpo_less> * sourceMap, bool generates ) {
	std::map<char*,char*,charTools::cmpo_less> * targetMap = sectionMap(sectionName, generates);
	if( targetMap != NULL ) {
		return sectionMapSet(targetMap, sourceMap);
	}
	return false;
}

/**
* Function: sectionMapSet
* Clones the various "name=value" pairs into a specified section, from the map provided
*
* Returns:
* return true if value is set, else false (generates = false & section does not exists)
*
* Parameters:
* sectionName - The section name which the value belongs to.
* sourceMap - map source to copy values from
* generates - Indicates if the section is to be created if it does not exists (default as true)
**/
bool iniParser::sectionMapSet( std::map<char*,char*,charTools::cmpo_less> * targetMap, std::map<char*,char*,charTools::cmpo_less> * sourceMap ) {
	if( targetMap == NULL || sourceMap == NULL ) {
		return false;
	}
	
	std::map<char*, char*, charTools::cmpo_less>::iterator nameValue;
	for( nameValue=sourceMap->begin(); nameValue != sourceMap->end(); nameValue++ ) {
		sectionValueSet( targetMap, nameValue->first, nameValue->second );
	}
	return true;
}

/**
* Function: ~iniParser
* Class destructor, which calls <iniParser.clear> to prevent any memory leaks.
**/
iniParser::~iniParser() {
	clear();
}

/**
* Function: clear
* Does the garbage collection and clean up of <iniParser>, ensuring there are no leaks nor data
**/
void iniParser::clear() {
	std::map<char*, std::map<char*,char*,charTools::cmpo_less> *, charTools::cmpo_less >::iterator sectMap;
	std::map<char*, char*, charTools::cmpo_less>::iterator valueMap;
	
	//itinerate over each section
	for ( sectMap=storedConfig.begin(); sectMap != storedConfig.end(); sectMap++ ) {
		//itinerate over each value (inside a section)
		for( valueMap=(*((*sectMap).second)).begin(); valueMap != (*((*sectMap).second)).end(); valueMap++ ) {
			delete ((*valueMap).second); //delete the stored value
			delete ((*valueMap).first); //delete the value name
		}
		(*((*sectMap).second)).clear(); //nuke the various stored name=value pairs
		//All section values would have been cleared by here
		delete ((*sectMap).second); //delete the section map
		delete ((*sectMap).first); //delete the section name
	}
	storedConfig.clear(); //nukes the various configuration names and submaps
}
