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
#include "charTools.hpp"
#include "charMacros.h"

#include <string.h>
#include <stdlib.h>
#include <climits>

/************************************************************************************
* Class: charTools
* Various string helper tools that is sometimes used repeatedly
************************************************************************************/

/************************************************************************************
* Function: newClone
* Creates a "new" clone of the input char
************************************************************************************/
char* charTools::newClone(const char* inChar) {
	return strcpy( new char[ strlen(inChar) + 1 ], inChar );
}

/************************************************************************************
* Function: toBool
* Converts the char* to its bool value for cases of 1/0, true/false, yes/no, or part thereof.
*
* Parameters:
* const char* inChar - input char* to check
* bool defaultBool - bool value to output, if no pattern matches
************************************************************************************/
bool charTools::toBool(const char* inChar, bool defaultBool) {
	int_fast8_t res = charTools::_toBool(inChar);

	if(res > 0) {
		return true;
	} else if(res < 0) {
		return false;
	} else {
		return defaultBool;
	}
}

/************************************************************************************
* Function: _toBool
* Converts the char* to its int "bool" value for cases of 1/0, true/false, yes/no, or part thereof.
* Note that this varient, is useful when failure / nomatch is an option.
*
* Parameters:
* const char* inChar - input char* to check
*
* Returns:
* 0 - Failed to match any bool value pattern
* 1 - True pattern matched
* -1 - False pattern matched
************************************************************************************/
int_fast8_t charTools::_toBool(const char* inChar) {
	switch( inChar[0] ) {
		case CHAR_NUMBER_1: //1
		case CHAR_LETTER_Y: //Yes
		case CHAR_LETTER_Y_s:
		case CHAR_LETTER_T: //True
		case CHAR_LETTER_T_s:
			return 1;

		case CHAR_NUMBER_0: //0
		case CHAR_LETTER_N: //No
		case CHAR_LETTER_N_s:
		case CHAR_LETTER_F: //False
		case CHAR_LETTER_F_s:
			return -1;

		default: //nothing found
			return 0;
	}
}

/************************************************************************************
* Function: toLong
* Converts the char* to base 10 long value if possible : this is an inline referance to std strtol
*
* Parameters:
* const char* inChar - input char* to check
*
* Returns:
* 0 - No valid value / Is really 0
* LONG_MAX - Maximum long value, due to out-of-bound char value
* LONG_MIN - Minimum long value, due to out-of-bound char value
************************************************************************************/
long charTools::toLong(const char* inChar) {
	return strtol(inChar, NULL, 10);
}

/************************************************************************************
* Class: charTools::cmpo_less
* cmpo_less = strcmp operators less
*
* Useful comparision operator that can be used for maps.
*
* Example:
* > std::map<char*,char*, charTools::cmpo_less> charsMap
************************************************************************************/
bool const charTools::cmpo_less::operator() (const char* lhs, const char* rhs) {
	return (strcmp(lhs,rhs) < 0);
}
